#pragma once


#include <QMainWindow>
#include <QFileDialog>
#include <QFileSystemWatcher>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarth/Viewpoint>
#include <osgEarth/EarthManipulator>	
#include <osgEarth/Utils>
#include <osgViewer/Viewer>
#include <osgQOpenGL/osgQOpenGLWidget>
#include <UiUpdateAble.h>
#include <FileLoaderListener.h>

class MainWindow : public QMainWindow , public FileLoaderListener
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void onFileLoadingStart(const QString& fileName) override {};
	void onFileLoadingEnd(const QString& fileName, bool success) override;
	void onFileReloadingStart() override {};
	void onFileReloadingEnd(bool success) override {};

	QFileSystemWatcher fileWatcher;

	void addUpdateAble(UiUpdateAble* listener);
	void removeUpdateAble(UiUpdateAble* listener);

public slots:
	void onCurrentFileChanged();

private slots:
	void notifyListeners();

private:
	QTimer* updateTimer;                // Timer for periodic updates
	QList<UiUpdateAble*> listeners;     // List of registered listeners
};
