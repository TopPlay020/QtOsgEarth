#pragma once

#include "globals.h"
#include "FileLoaderListener.h"
#include <QFileSystemWatcher>
#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include <QLabel>

class MenuManager : public QObject, public FileLoaderListener, public UiUpdateAble {
public:
	void createMenu();

	void onFileLoadingStart(const QString& fileName) override;
	void onFileLoadingEnd(const QString& fileName, bool success) override;

	void onClose();

	void updateMouseGeoPointCoordinates(double longitude, double latitude, double altitude);

	void updateUi() override;
private:
	void setupFileMenu();
	void setupToolBar();
	void setupRecentFilesMenu();
	void setupStatusBar();

	void refreshRecentFilesMenuWithNewFileName(QString fileName);


	QToolBar* toolbar;
	QMenu* recentFilesMenu;

	QAction* a_openFile;
	QAction* a_saveFile;
	QAction* a_editFile;
	QAction* a_revealInFileExplorer;

	QAction* a_addLocation;
	QAction* a_addDistance;
	QAction* a_addLayer;
	QAction* a_add3D;

	QLabel* activeFileNameLabel;
	QLabel* mouseLatitudeLabel;
	QLabel* mouseLongitudeLabel;
	QLabel* mouseAltitudeLabel;

	QString oldFileName = nullptr;
	QFileSystemWatcher fileWatcher;

private slots:
	void onLoadActionClick();
	void onEditActionClick();
	void onRevealFileExplorerActionClick();
	void onCurrentFileChanged();
	void onSaveAsClick();
};
