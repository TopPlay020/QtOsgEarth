#include "globals.h"
#include <QDesktopServices>
#include <QTimer>
#include <QHBoxLayout>


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent)
{
	this->setWindowIcon(g_mediaManager->getIcon("Earth"));

	g_mainWindow = this;
	g_menuManager = new MenuManager();
	g_osgEarthManager = new OsgEarthManager();
	g_contextMenuManager = new ContextMenuManager();
	g_osgTreeViewManager = new OsgTreeViewManager();

	g_menuManager->createMenu();
	g_osgEarthManager->setupOsgEarth();
	g_contextMenuManager->createContextMenu();
	g_osgTreeViewManager->createTreeView();

	installEventFilter(g_globalEventProvider);




	auto sideBar = new QWidget();
	auto sideBar_layout = new QHBoxLayout();
	sideBar->setLayout(sideBar_layout);
	sideBar_layout->addWidget(g_osgTreeViewManager);
	sideBar_layout->addStretch();
	sideBar_layout->setContentsMargins(0, 0, 0, 0);
	//set sideBar fit explorer

	auto mainContainer = new QWidget();
	auto mainContainer_layout = new QHBoxLayout();
	mainContainer->setLayout(mainContainer_layout);
	mainContainer_layout->addWidget(sideBar);
	mainContainer_layout->addWidget(g_osgEarthManager);
	mainContainer_layout->setStretch(1, 1);


	

	setCentralWidget(mainContainer);



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