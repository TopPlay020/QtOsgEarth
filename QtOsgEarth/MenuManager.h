#pragma once

#include "globals.h"
#include "FileLoaderListener.h"
#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include <QLabel>

class MenuManager : public QObject, public FileLoaderListener, public UiUpdateAble {
public:
	void createMenu();

	void onFileLoadingStart(const QString& fileName) override;
	void onFileLoadingEnd(const QString& fileName, bool success) override;
	void onFileReloadingStart() override {};
	void onFileReloadingEnd(bool success) override {};

	void onClose();

	void updateMouseGeoPointCoordinates(double longitude, double latitude, double altitude);

	void updateUi() override;
private:
	void setupFileMenu();
	void setupRecentFilesMenu();
	void setupStatusBar();




	QMenu* fileMenu;
	QMenu* recentFilesMenu;
	QMenu* viewerContextMenu;

	QAction* loadAction;
	QAction* editAction;
	QAction* revealFileExplorerAction;

	QLabel* activeFileNameLabel;
	QLabel* mouseLatitudeLabel;
	QLabel* mouseLongitudeLabel;
	QLabel* mouseAltitudeLabel;

private slots:
	void onLoadActionClick();
	void onEditActionClick();
	void onRevealFileExplorerActionClick();
};