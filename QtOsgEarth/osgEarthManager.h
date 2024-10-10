#pragma once

#include "globals.h"
#include "FileLoaderProvider.h"
#include <OsgEarthUtility.h>

class OsgEarthManager : public osgQOpenGLWidget, public FileLoaderProvider
{
public:
	void setupOsgEarth();

	void loadEarthFile(const QString& fileName);
	void saveEarthFile(const QString& fileName);
	void reloadEarthFile();

	osg::Vec3d getMouseCoordinatesInGeoPoint(int mouseX, int mouseY);
	double getAltitude(double latitude, double longitude);
	void adjustAltitudes(QList<osg::Vec3d>& points);

	osgEarth::LayerVector getLayers();
	
	OsgLabel addLabel(osg::Vec3d point, QString text);
	OsgLine addLine(osg::Vec3d pointStart, osg::Vec3d pointEnd);
	OsgPolygon addPolygon(QList<osg::Vec3d>& points);
	void removeNode(OsgNodeAdapter& nodeAdapter) { removeNode(nodeAdapter.getNode()); };
	void removeNode(osg::Node* node);

	void onClose();
public:
	QString activeFile;
private:
	void initOsg();
	void setupSky();

	osgViewer::Viewer* viewer;
	osgEarth::MapNode* mapNode;
	osgEarth::EarthManipulator* manip;
	osg::ref_ptr<osg::Node> earthNode;
	osg::Group* root;
};

