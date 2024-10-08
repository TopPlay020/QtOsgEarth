#pragma once


#include <QMainWindow>
#include <QFileDialog>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarth/Viewpoint>
#include <osgEarth/EarthManipulator>	
#include <osgEarth/Utils>
#include <osgViewer/Viewer>
#include <osgQOpenGL/osgQOpenGLWidget>
#include <UiUpdateAble.h>
#include <FileLoaderListener.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	
	void addUpdateAble(UiUpdateAble* listener);
	void removeUpdateAble(UiUpdateAble* listener);

private slots:
	void notifyListeners();

private:
	QTimer* updateTimer;                // Timer for periodic updates
	QList<UiUpdateAble*> listeners;     // List of registered listeners
};
