#include <globals.h>
#include <QDesktopServices>
#include <QSettings>
#include <QStatusBar>

void MenuManager::createMenu() {
	setupFileMenu();
	setupRecentFilesMenu();
	setupStatusBar();
	g_mainWindow->addUpdateAble(this);
	g_osgEarthManager->addFileLoaderListener(this);
}


void MenuManager::setupFileMenu() {
	fileMenu = g_mainWindow->menuBar()->addMenu("&File");

	loadAction = new QAction("&Open File...", g_mainWindow);
	loadAction->setIcon(g_mediaManager->getIcon("File"));
	QObject::connect(loadAction, &QAction::triggered, this, &MenuManager::onLoadActionClick);
	fileMenu->addAction(loadAction);

	recentFilesMenu = new QMenu("Open Recent File", g_mainWindow);
	recentFilesMenu->setIcon(g_mediaManager->getIcon("RecentFile"));
	fileMenu->addMenu(recentFilesMenu);

	fileMenu->addSeparator();

	editAction = fileMenu->addAction("&Edit File");
	editAction->setIcon(g_mediaManager->getIcon("Edit"));
	QObject::connect(editAction, &QAction::triggered, this, &MenuManager::onEditActionClick);
	editAction->setEnabled(false);

	revealFileExplorerAction = fileMenu->addAction("&reveal in File Explorer");
	revealFileExplorerAction->setIcon(g_mediaManager->getIcon("Folder"));
	QObject::connect(revealFileExplorerAction, &QAction::triggered, this, &MenuManager::onRevealFileExplorerActionClick);
	revealFileExplorerAction->setEnabled(false);
}

void MenuManager::setupRecentFilesMenu() {
	QSettings settings("EMP", "QtOsgEarth");

	// Load the recent files list from settings
	QStringList recentFiles = settings.value("recentFiles").toStringList();

	// Clear old entries
	recentFilesMenu->clear();

	for (const QString& file : recentFiles) {
		QAction* action = recentFilesMenu->addAction(file);
		action->setIcon(g_mediaManager->getIcon("File"));
		QObject::connect(action, &QAction::triggered, g_mainWindow, [this, file]() {
			g_osgEarthManager->loadEarthFile(file);
			});
	}
}

void MenuManager::setupStatusBar() {
	// Create labels for latitude and longitude
	activeFileNameLabel = new QLabel("Please Select File First", g_mainWindow);
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
	fileMenu->setEnabled(false);
}

void MenuManager::onFileLoadingEnd(const QString& fileName, bool success) {

	fileMenu->setEnabled(true);
	revealFileExplorerAction->setEnabled(true);
	editAction->setEnabled(true);

	//change label in status bar 
	activeFileNameLabel->setText(QFileInfo(fileName).fileName());

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
	for (auto action : actions) {
		if (action->text() == fileName)
			return;  // File is already in the list
	}

	// Create a new action for the file
	QAction* action = recentFilesMenu->addAction(fileName);
	action->setIcon(g_mediaManager->getIcon("File"));

	// Connect the action to load the file when triggered
	QObject::connect(action, &QAction::triggered, g_mainWindow, [this, fileName]() {
		g_osgEarthManager->loadEarthFile(fileName);
		});

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