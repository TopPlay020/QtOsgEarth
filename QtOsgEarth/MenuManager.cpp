#include <globals.h>
#include <QDesktopServices>
#include <QSettings>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>


void MenuManager::createMenu() {
	//setupFileMenu();
	setupToolBar();
	setupRecentFilesMenu();
	setupStatusBar();
	g_mainWindow->addUpdateAble(this);
	g_osgEarthManager->addFileLoaderListener(this);
}


void MenuManager::setupFileMenu() {
	/*fileMenu = g_mainWindow->menuBar()->addMenu("&File");

	loadAction = new QAction("&Open File...", g_mainWindow);
	loadAction->setIcon(g_mediaManager->getIcon("File"));
	QObject::connect(loadAction, &QAction::triggered, this, &MenuManager::onLoadActionClick);
	fileMenu->addAction(loadAction);

	recentFilesMenu = new QMenu("Open Recent File", g_mainWindow);
	recentFilesMenu->setIcon(g_mediaManager->getIcon("RecentFile"));
	fileMenu->addMenu(recentFilesMenu);

	fileMenu->addSeparator();

	saveAsAction = fileMenu->addAction("&Save As");
	saveAsAction->setIcon(g_mediaManager->getIcon("saveAs"));
	QObject::connect(saveAsAction, &QAction::triggered, this, &MenuManager::onSaveAsClick);
	saveAsAction->setEnabled(false);

	fileMenu->addSeparator();

	editAction = fileMenu->addAction("&Edit File");
	editAction->setIcon(g_mediaManager->getIcon("Edit"));
	QObject::connect(editAction, &QAction::triggered, this, &MenuManager::onEditActionClick);
	editAction->setEnabled(false);

	revealFileExplorerAction = fileMenu->addAction("&reveal in File Explorer");
	revealFileExplorerAction->setIcon(g_mediaManager->getIcon("Folder"));
	QObject::connect(revealFileExplorerAction, &QAction::triggered, this, &MenuManager::onRevealFileExplorerActionClick);
	revealFileExplorerAction->setEnabled(false);*/
}


void MenuManager::setupToolBar() {
	// Create the toolbar
	toolbar = new QToolBar("Main Toolbar", g_mainWindow);
	toolbar->setMovable(false);

	// Set the style to display text under icons
	toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	// Set fixed width for the buttons
	auto createFixedWidthButton = [](QAction* action) {
		QToolButton* button = new QToolButton;
		button->setDefaultAction(action);
		button->setFixedWidth(60);
		button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		return button;
		};

	// Create a menu for recent files
	recentFilesMenu = new QMenu;

	// Create a tool button for "Recents" with a popup menu
	QToolButton* recentFilesButton = new QToolButton;
	recentFilesButton->setIcon(g_mediaManager->getIcon("recent-file"));
	recentFilesButton->setText("Recents");
	recentFilesButton->setPopupMode(QToolButton::InstantPopup);  // Open menu on click
	recentFilesButton->setMenu(recentFilesMenu);
	recentFilesButton->setFixedWidth(60);
	recentFilesButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	recentFilesButton->setStyleSheet("QToolButton::menu-indicator {width: 0px;height: 0px;}");


	a_openFile = new QAction(g_mediaManager->getIcon("open-file"), "Open File...", g_mainWindow);
	a_saveFile = new QAction(g_mediaManager->getIcon("save-file"), "Save As", g_mainWindow);
	a_editFile = new QAction(g_mediaManager->getIcon("edit-file"), "Edit File", g_mainWindow);
	a_revealInFileExplorer = new QAction(g_mediaManager->getIcon("open-folder"), "Folder", g_mainWindow);

	a_revealInFileExplorer->setEnabled(false);
	a_editFile->setEnabled(false);
	a_saveFile->setEnabled(false);

	toolbar->addWidget(createFixedWidthButton(a_openFile));
	toolbar->addWidget(recentFilesButton);
	toolbar->addWidget(createFixedWidthButton(a_saveFile));
	toolbar->addWidget(createFixedWidthButton(a_editFile));
	toolbar->addWidget(createFixedWidthButton(a_revealInFileExplorer));

	QObject::connect(a_openFile, &QAction::triggered, this, &MenuManager::onLoadActionClick);
	QObject::connect(a_saveFile, &QAction::triggered, this, &MenuManager::onSaveAsClick);
	QObject::connect(a_editFile, &QAction::triggered, this, &MenuManager::onEditActionClick);
	QObject::connect(a_revealInFileExplorer, &QAction::triggered, this, &MenuManager::onRevealFileExplorerActionClick);


	toolbar->addSeparator();

	a_addLocation = new QAction(g_mediaManager->getIcon("add-location"), "Add Label", g_mainWindow);
	a_addDistance = new QAction(g_mediaManager->getIcon("add-distance"), "Distance", g_mainWindow);
	a_addLayer = new QAction(g_mediaManager->getIcon("add-layer"), "Add Layer", g_mainWindow);
	a_add3D = new QAction(g_mediaManager->getIcon("add-3d"), "Add 3D", g_mainWindow);

	a_addLocation->setEnabled(false);
	a_addDistance->setEnabled(false);
	a_addLayer->setEnabled(false);
	a_add3D->setEnabled(false);

	toolbar->addWidget(createFixedWidthButton(a_addLocation));
	toolbar->addWidget(createFixedWidthButton(a_addDistance));
	toolbar->addWidget(createFixedWidthButton(a_addLayer));
	toolbar->addWidget(createFixedWidthButton(a_add3D));

	// Add the toolbar to the main window
	g_mainWindow->addToolBar(Qt::TopToolBarArea, toolbar);
}



void MenuManager::setupRecentFilesMenu() {
	QSettings settings("EMP", "QtOsgEarth");

	// Load the recent files list from settings
	QStringList recentFiles = settings.value("recentFiles").toStringList();

	// Clear old entries
	recentFilesMenu->clear();

	QStringList validFiles;

	for (const QString& file : recentFiles) {
		if (QFile::exists(file)) {  // Check if the file still exists
			QAction* action = recentFilesMenu->addAction(file);
			action->setIcon(g_mediaManager->getIcon("File"));
			QObject::connect(action, &QAction::triggered, g_mainWindow, [this, file]() {
				g_osgEarthManager->loadEarthFile(file);
				});

			validFiles.append(file);  // Keep valid files in the list
		}
	}

	// Save the updated valid files list back to settings
	settings.setValue("recentFiles", validFiles);
}

void MenuManager::setupStatusBar() {
	// Create labels for latitude and longitude
	activeFileNameLabel = new QLabel("   Please Select File First", g_mainWindow);
	mouseLatitudeLabel = new QLabel("Latitude : 0.0", g_mainWindow);
	mouseLongitudeLabel = new QLabel("Longitude: 0.0", g_mainWindow);
	mouseAltitudeLabel = new QLabel("Altitude : 0.0", g_mainWindow);

	// fix width 
	mouseLatitudeLabel->setFixedWidth(150);
	mouseLongitudeLabel->setFixedWidth(150);
	mouseAltitudeLabel->setFixedWidth(150);

	// Add the labels to the status bar
	g_mainWindow->statusBar()->addWidget(activeFileNameLabel);
	g_mainWindow->statusBar()->addPermanentWidget(mouseLatitudeLabel);
	g_mainWindow->statusBar()->addPermanentWidget(mouseLongitudeLabel);
	g_mainWindow->statusBar()->addPermanentWidget(mouseAltitudeLabel);

	// Make the activeFileNameLabel take the remaining width
	g_mainWindow->statusBar()->addWidget(activeFileNameLabel, 1);
}

void MenuManager::updateMouseGeoPointCoordinates(double longitude, double latitude, double altitude) {
	mouseLatitudeLabel->setText(QString("Latitude: %1").arg(latitude, 0, 'f', 4));
	mouseLongitudeLabel->setText(QString("Longitude: %1").arg(longitude, 0, 'f', 4));
	mouseAltitudeLabel->setText(QString("Altitude: %1").arg(altitude, 0, 'f', 4));
}

void MenuManager::onFileLoadingStart(const QString& fileName) {
	toolbar->setEnabled(false);
}

void MenuManager::onFileLoadingEnd(const QString& fileName, bool success) {

	toolbar->setEnabled(true);
	a_revealInFileExplorer->setEnabled(true);
	a_editFile->setEnabled(true);
	a_saveFile->setEnabled(true);

	//change label in status bar 
	activeFileNameLabel->setText("   " + QFileInfo(fileName).fileName());

	refreshRecentFilesMenuWithNewFileName(fileName);

	if (!oldFileName.isEmpty())
		fileWatcher.removePath(oldFileName);

	oldFileName = g_osgEarthManager->activeFile;
	fileWatcher.addPath(g_osgEarthManager->activeFile);

	connect(&fileWatcher, &QFileSystemWatcher::fileChanged, this, &MenuManager::onCurrentFileChanged);

}

void MenuManager::refreshRecentFilesMenuWithNewFileName(QString fileName) {

	// Check if the file is already in the list
	QList<QAction*> actions = recentFilesMenu->actions();
	QAction* existingAction = nullptr;
	if (!actions.isEmpty())
		for (auto action : actions) {
			if (action->text() == fileName) {
				existingAction = action;  // File found in the list
				break;
			}
		}

	if (existingAction) {
		// Move the existing action to the top
		recentFilesMenu->removeAction(existingAction);
		recentFilesMenu->insertAction(actions.isEmpty() ? nullptr : actions.first(), existingAction);
	}
	else {
		// Create a new action for the file
		QAction* action = recentFilesMenu->addAction(fileName);
		action->setIcon(g_mediaManager->getIcon("File"));

		// Connect the action to load the file when triggered
		QObject::connect(action, &QAction::triggered, g_mainWindow, [this, fileName]() {
			g_osgEarthManager->loadEarthFile(fileName);
			});
	}

	// Optionally limit the list size
	const int maxRecentFiles = 10;
	if (recentFilesMenu->actions().size() > maxRecentFiles) {
		delete recentFilesMenu->actions().last();
	}
}

void MenuManager::onLoadActionClick() {
	QString fileName = QFileDialog::getOpenFileName(g_mainWindow, "Open Earth File", "", "*.earth");
	if (!fileName.isEmpty())
		g_osgEarthManager->loadEarthFile(fileName);

}

void MenuManager::onEditActionClick() {
	QDesktopServices::openUrl(QUrl::fromLocalFile(g_osgEarthManager->activeFile));
}

void MenuManager::onRevealFileExplorerActionClick() {
	QFileInfo fileInfo(g_osgEarthManager->activeFile);
	QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
}

void MenuManager::onSaveAsClick() {
	QString fileName = QFileDialog::getSaveFileName(g_mainWindow, "Save Earth File", "", "*.earth");
	if (!fileName.isEmpty()) {
		g_osgEarthManager->saveEarthFile(fileName);
	}
}

void MenuManager::onClose() {
	QSettings settings("EMP", "QtOsgEarth");

	// Save the list of recent files
	QStringList recentFiles;
	for (auto action : recentFilesMenu->actions()) {
		recentFiles.append(action->text());
	}
	settings.setValue("recentFiles", recentFiles);
}

void MenuManager::onCurrentFileChanged() {
	g_osgEarthManager->reloadEarthFile();
}

void MenuManager::updateUi() {
	QPoint globalMousePos = QCursor::pos();
	QPoint localMousePos = g_osgEarthManager->mapFromGlobal(globalMousePos);

	int mouseX = localMousePos.x();
	int mouseY = localMousePos.y();

	//auto geoPoint = g_osgEarthManager->getPixelPosition(mouseX, mouseY);
	auto geoPoint = g_osgEarthManager->getMouseCoordinatesInGeoPoint(mouseX, mouseY);

	updateMouseGeoPointCoordinates(geoPoint.x(), geoPoint.y(), geoPoint.z());





}