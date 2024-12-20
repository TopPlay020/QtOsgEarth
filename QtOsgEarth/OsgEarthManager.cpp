#include "globals.h"
#include <QTimer>
#include <osgEarth/GLUtils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgEarth/Sky>
#include <QString>
#include <osg/LineSegment>
#include <osgUtil/LineSegmentIntersector>
#include <osgEarth/Feature>
#include <osgEarth/FeatureNode>
#include <osgEarth/ElevationQuery>
#include <osgEarth/AnnotationLayer>


void OsgEarthManager::setupOsgEarth() {
	QTimer::singleShot(1000, g_mainWindow, [this]() {
		initOsg();
		});
}

void OsgEarthManager::initOsg() {
	while (!getOsgViewer());//wait until initialized
	viewer = getOsgViewer();

	osgEarth::GLUtils::setGlobalDefaults(viewer->getCamera()->getOrCreateStateSet());

	//viewer->setRealizeOperation(new GL3RealizeOperation());
	viewer->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);
	viewer->getCamera()->setNearFarRatio(0.00002);
	viewer->getCamera()->setClearColor(osg::Vec4f(0.47, 0.78, 1.0, 1));

	// Set up basic OSG viewer settings
	viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
	manip = new osgEarth::EarthManipulator();
	viewer->setCameraManipulator(manip);

	root = new osg::Group;
	viewer->setSceneData(root);
	installEventFilter(g_globalEventProvider);


}

void OsgEarthManager::loadEarthFile(const QString& fileName) {
	bool isReloding = fileName == activeFile;
	if (isReloding)
		notifyFileReloadingStart();
	else
		notifyFileLoadingStart(fileName);

	// Clear previous nodes if they exist
	if (mapNode)
	{
		root->removeChild(mapNode);
		mapNode = nullptr; // Reset the mapNode pointer
	}

	if (earthNode)
	{
		root->removeChild(earthNode);
		earthNode = nullptr; // Reset the earthNode pointer
	}

	// Load the .earth file using osgEarth
	if (fileName.endsWith(".earth"))
		earthNode = osgDB::readNodeFile(fileName.toStdString());
	else
		earthNode = osgDB::readNodeFile("D:/Tools_GIS/QtOsgEarthv2.0/default.earth");

	if (earthNode)
	{
		// Cast the loaded node to a MapNode
		mapNode = osgEarth::Util::MapNode::get(earthNode.get());
		if (mapNode)
			setupSky();
		root->addChild(earthNode);
	}
	if (fileName.endsWith(".osgBegh"))
		for (auto layer : g_osgSqlLiteManager->loadSqlFile(fileName.toStdString())) {
			addLayer(layer);
		}

	activeFile = fileName;
	if (isReloding)
		notifyFileReloadingEnd(true);
	else
		notifyFileLoadingEnd(fileName, true);

}

void OsgEarthManager::saveEarthFile(const QString& fileName) {
	if (earthNode) {
		std::string outputFileName = fileName.toStdString();

		// Check if the filename ends with ".earth" and append if necessary
		std::size_t dotPosition = outputFileName.find_last_of('.');
		if (dotPosition != std::string::npos)
			outputFileName = outputFileName.substr(0, dotPosition);  // Remove current extension

		outputFileName += ".earth";

		osgDB::Options* options = nullptr;
		osgDB::writeNodeFile(*earthNode, outputFileName, options);
	}
}

//By Me
osgEarth::GeoPoint OsgEarthManager::getMouseCoordinatesInGeoPoint(int mouseX, int mouseY) {
	if (!viewer || !mapNode) {
		return nullptr; // Assuming you want to keep the third dimension as 0
	}

	// Get the viewport and camera
	osg::Camera* camera = viewer->getCamera();
	osg::Viewport* viewport = camera->getViewport();

	// Convert mouse coordinates to normalized device coordinates (NDC)
	double ndcX = (2.0 * mouseX) / geometry().width() - 1.0;
	double ndcY = (2.0 * (geometry().height() - mouseY)) / geometry().height() - 1.0;

	// Create a ray from the camera through the mouse position
	osg::Matrixd inverseProjectionMatrix = osg::Matrixd::inverse(camera->getProjectionMatrix());
	osg::Matrixd inverseViewMatrix = osg::Matrixd::inverse(camera->getViewMatrix());

	osg::Vec3d nearPoint = osg::Vec3d(ndcX, ndcY, -1.0) * inverseProjectionMatrix * inverseViewMatrix;
	osg::Vec3d farPoint = osg::Vec3d(ndcX, ndcY, 1.0) * inverseProjectionMatrix * inverseViewMatrix;

	// Perform intersection test
	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(nearPoint, farPoint);
	osgUtil::IntersectionVisitor iv(intersector);
	viewer->getCamera()->accept(iv);

	if (intersector->containsIntersections()) {
		osgUtil::LineSegmentIntersector::Intersection intersection = intersector->getFirstIntersection();
		osg::Vec3d worldPos = intersection.getWorldIntersectPoint();
		osgEarth::GeoPoint geoPoint;
		geoPoint.fromWorld(mapNode->getMapSRS(), worldPos);
		return geoPoint;
	}

	return nullptr;
}

osg::Vec3d OsgEarthManager::getMouseCoordinates(int mouseX, int mouseY) {
	auto geoPoint = getMouseCoordinatesInGeoPoint(mouseX, mouseY);
	if (geoPoint == nullptr)
		return osg::Vec3d(200, 200, 200);
	double longitude = geoPoint.x();
	double latitude = geoPoint.y();
	double altitude = geoPoint.z();
	return osg::Vec3d(longitude, latitude, altitude);
}

double OsgEarthManager::getAltitude(double latitude, double longitude)
{
	if (!mapNode)
		return 0.0;

	osgEarth::GeoPoint geoPoint(mapNode->getMapSRS(), longitude, latitude, osgEarth::ALTMODE_ABSOLUTE);
	osgEarth::ElevationQuery query(mapNode->getMap());

	return query.getElevation(geoPoint);
}

void OsgEarthManager::adjustAltitudes(QList<osg::Vec3d>& points) {
	if (!mapNode)
		return;
	osgEarth::ElevationQuery query(mapNode->getMap());
	std::vector<osg::Vec3d> pointsWithAltitude;
	for (const osg::Vec3d& point : points)
		pointsWithAltitude.push_back(point);
	query.getElevations(pointsWithAltitude, mapNode->getMapSRS());
	for (int i = 0; i < pointsWithAltitude.size(); ++i) {
		points[i].z() = pointsWithAltitude[i].z();
	}

}

osgEarth::LayerVector OsgEarthManager::getLayers() {
	osg::Group* group = earthNode->asGroup();
	for (unsigned int i = 0; i < group->getNumChildren(); ++i)
	{
		osg::Node* childNode = group->getChild(i);
		if (childNode && QString::fromStdString(childNode->getName()) != "")
		{
			osgEarth::Util::MapNode* mapNode = osgEarth::Util::MapNode::get(childNode);
			if (mapNode)
			{
				osgEarth::Map* map = mapNode->getMap();
				auto layerVectors = osgEarth::LayerVector();
				map->getLayers(layerVectors);
				return layerVectors;
			}

		}
	}
}

OsgLabel OsgEarthManager::addLabel(osg::Vec3d point, QString text)
{
	// Create a geographic position with the provided lat, long, alt
	osgEarth::GeoPoint geoPoint(mapNode->getMapSRS(), point.x(), point.y(), point.z(), osgEarth::ALTMODE_ABSOLUTE);

	// Create a style for the point
	osgEarth::Style style;

	// Set the icon for the style
	osgEarth::IconSymbol* icon = style.getOrCreate<osgEarth::IconSymbol>();
	icon->url()->setLiteral(g_mediaManager->getIconPath(g_mediaManager->getIcon("PlaceHolder")));
	icon->declutter() = true; // Optional: to avoid overlapping icons

	// Create a place node (annotation)
	osgEarth::PlaceNode* placeNode = new osgEarth::PlaceNode(geoPoint, text.toStdString(), style);

	auto layer = new osgEarth::AnnotationLayer();
	layer->setName(text.toStdString());
	layer->addChild(placeNode);
	addLayer(layer);

	return OsgLabel(placeNode, layer);
}

OsgLine OsgEarthManager::addLine(osg::Vec3d pointStart, osg::Vec3d pointEnd) {
	if (!mapNode || !root)
		return nullptr;

	// Create geographic positions for the start and end points
	osgEarth::GeoPoint geoPointStart(mapNode->getMapSRS(), pointStart.x(), pointStart.y(), pointStart.z(), osgEarth::ALTMODE_ABSOLUTE);
	osgEarth::GeoPoint geoPointEnd(mapNode->getMapSRS(), pointEnd.x(), pointEnd.y(), pointEnd.z(), osgEarth::ALTMODE_ABSOLUTE);

	// Create a feature representing the line
	osgEarth::LineString* line = new osgEarth::LineString();
	line->push_back(geoPointStart.vec3d());
	line->push_back(geoPointEnd.vec3d());

	// Create a style for the line
	osgEarth::Style style;
	osgEarth::LineSymbol* lineSymbol = style.getOrCreate<osgEarth::LineSymbol>();
	lineSymbol->stroke()->color() = osgEarth::Color::Red;
	lineSymbol->stroke()->width() = 5.0f;
	lineSymbol->tessellation() = 10;

	// Create a feature representing the line
	osgEarth::Feature* feature = new osgEarth::Feature(line, mapNode->getMapSRS());
	feature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

	// Create a feature node to render the line
	osgEarth::FeatureNode* featureNode = new osgEarth::FeatureNode(feature, style);

	// Add the feature node to the scene graph
	mapNode->addChild(featureNode);

	return OsgLine(featureNode);
}

OsgPolygon OsgEarthManager::addPolygon(QList<osg::Vec3d>& points) {
	if (!mapNode || !root)
		return nullptr;

	// Create a polygon geometry
	osgEarth::Polygon* polygon = new osgEarth::Polygon();

	// Add points to the polygon
	for (const osg::Vec3d& point : points) {
		osgEarth::GeoPoint geoPoint(mapNode->getMapSRS(), point.x(), point.y(), point.z(), osgEarth::ALTMODE_ABSOLUTE);
		polygon->push_back(geoPoint.vec3d());
	}

	// Create a style for the polygon
	osgEarth::Style style;
	osgEarth::LineSymbol* lineSymbol = style.getOrCreate<osgEarth::LineSymbol>();
	lineSymbol->stroke()->color() = osgEarth::Color::Red;
	lineSymbol->stroke()->width() = 5.0f;
	lineSymbol->tessellation() = 10;

	// Create a feature representing the polygon
	osgEarth::Feature* feature = new osgEarth::Feature(polygon, mapNode->getMapSRS());
	feature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

	// Create a feature node to render the polygon
	osgEarth::FeatureNode* featureNode = new osgEarth::FeatureNode(feature, style);

	// Add the feature node to the scene graph
	mapNode->addChild(featureNode);

	return OsgPolygon(featureNode);
}

void OsgEarthManager::addLayer(osgEarth::Layer* layer) {
	mapNode->getMap()->addLayer(layer);
	notifyLayerAdd(layer);
}
void OsgEarthManager::removeLayer(osgEarth::Layer* layer) {
	notifyLayerRemove(layer);
	mapNode->getMap()->removeLayer(layer);
}

void OsgEarthManager::removeNode(osg::Node* node) {
	if (!mapNode || !root)
		return;
	if (mapNode->containsNode(node))
		mapNode->removeChild(node);
}


void OsgEarthManager::setupSky() {
	osg::ref_ptr<osgEarth::SkyNode> sky = osgEarth::SkyNode::create();
	sky->attach(viewer);
	sky->setSunVisible(true);
	sky->setMoonVisible(true);
	sky->setDateTime(osgEarth::DateTime(QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().day(), QTime::currentTime().hour()));
	sky->getSunLight()->setAmbient(osg::Vec4(0.5, 0.5, 0.5, 1.0));
	mapNode->addChild(sky);
}

void OsgEarthManager::reloadEarthFile() {
	osgEarth::Viewpoint vp;
	if (manip)
		vp = manip->getViewpoint();

	loadEarthFile(activeFile);
	if (manip)
		manip->setViewpoint(vp);
}

void OsgEarthManager::onClose() {
	delete viewer;
}