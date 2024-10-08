#include "globals.h"
#include <QDesktopServices>
#include <QTimer>


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent)
{
	this->setWindowIcon(g_mediaManager->getIcon("Earth"));

	g_mainWindow = this;
	g_menuManager = new MenuManager();
	g_menuManager->createMenu();

	g_osgEarthManager = new OsgEarthManager();
	setCentralWidget(g_osgEarthManager);
	g_osgEarthManager->setupOsgEarth();
	g_osgEarthManager->addFileLoaderListener(g_menuManager);

	installEventFilter(g_globalEventProvider);

	g_contextMenuManager = new ContextMenuManager();
	g_contextMenuManager->createContextMenu();

	//init Global Timer
	updateTimer = new QTimer(this);
	connect(updateTimer, &QTimer::timeout, this, &MainWindow::notifyListeners);
	updateTimer->start(1000 / 60);

}

MainWindow::~MainWindow()
{
	g_menuManager->onClose();
	g_osgEarthManager->onClose();
}


void MainWindow::addUpdateAble(UiUpdateAble* listener) {
	listeners.append(listener);
}

void MainWindow::removeUpdateAble(UiUpdateAble* listener) {
	listeners.removeAll(listener);
}

void MainWindow::notifyListeners() {
	for (UiUpdateAble* listener : listeners)
		listener->updateUi();
}