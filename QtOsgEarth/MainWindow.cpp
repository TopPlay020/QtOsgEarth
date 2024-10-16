#include "globals.h"
#include "OsgEarthContextMenu.h"
#include "OsgTreeViewContextMenu.h"
#include <QDesktopServices>
#include <QTimer>
#include <QHBoxLayout>
#include <QPropertyAnimation>


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent)
{
	this->setWindowIcon(g_mediaManager->getIcon("Earth"));

	g_mainWindow = this;
	g_menuManager = new MenuManager();
	g_osgEarthManager = new OsgEarthManager();
	g_osgTreeViewManager = new OsgTreeViewManager();
	g_osgEarthTaskExecutor = new OsgEarthTaskExecutor();

	g_menuManager->createMenu();
	g_osgEarthManager->setupOsgEarth();
	g_osgTreeViewManager->createTreeView();

	(new OsgEarthContextMenu())->createContextMenu();
	(new OsgTreeViewContextMenu())->createContextMenu();

	installEventFilter(g_globalEventProvider);

	setupUi();

	//init Global Timer
	updateTimer = new QTimer(this);
	connect(updateTimer, &QTimer::timeout, this, &MainWindow::notifyListeners);
	updateTimer->start(1000 / 60);

	hideLeftSideBar();

}

void MainWindow::setupUi() {
	leftSideBar = new QWidget();
	auto leftSideBar_layout = new QHBoxLayout();
	leftSideBar->setLayout(leftSideBar_layout);
	leftSideBar_layout->addWidget(g_osgTreeViewManager);
	leftSideBar_layout->addStretch();
	leftSideBar_layout->setContentsMargins(0, 0, 0, 0);

	auto mainContainer = new QWidget();
	auto mainContainer_layout = new QHBoxLayout();
	mainContainer->setLayout(mainContainer_layout);
	mainContainer_layout->setSpacing(0);
	mainContainer_layout->addWidget(leftSideBar);
	mainContainer_layout->addWidget(g_osgEarthManager);
	mainContainer_layout->setStretch(1, 1);
	mainContainer_layout->setContentsMargins(0, 0, 0, 0);


	setCentralWidget(mainContainer);
}

void MainWindow::showLeftSideBar() {
	leftSideBar->show();
}

void MainWindow::hideLeftSideBar() {
	leftSideBar->hide();
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