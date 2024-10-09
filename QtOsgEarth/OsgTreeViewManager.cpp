#include "globals.h"

void OsgTreeViewManager::createTreeView() {
	g_osgEarthManager->addFileLoaderListener(this);
	setupView();
}

void OsgTreeViewManager::setupView() {
	this->setHeaderHidden(true);

	// Create a standard item model
	auto model = new QStandardItemModel;

	// Create root node
	auto rootItem = model->invisibleRootItem();

	// Create nodes
	rootNode = new QStandardItem("Planat");

	// Set icons for each item
	rootNode->setIcon(g_mediaManager->getIcon("globe-earth"));

	// Add nodes to the root
	rootItem->appendRow(rootNode); // Add Root node

	// Make items unselectable
	rootNode->setFlags(Qt::ItemIsEnabled);

	this->setModel(model);

	QFont font = this->font();
	font.setPointSize(12);
	this->setFont(font);
	QSize iconSize(28, 28);
	this->setIconSize(iconSize);

	// Apply styles
	this->setStyleSheet(
		"QTreeView {"
		"    border: 1px solid #999;" // Border for the tree view
		"    selection-background-color: #a4c8f0;" // Selection color
		"    selection-color: #fff;" // Selection text color
		"    outline: 0;"
		"}"
		"QTreeView::item {"
		"    padding: 5px;" // Padding for items
		"}"
		"QTreeView::item:hover {"
		"    background-color: #e3f2fd;" // Hover color for items
		"}"
		"QTreeView::item:selected {"
		"    background-color: #a4c8f0;" // Selected item background color
		"}"

	);
}

OsgTreeViewManager::LayerType OsgTreeViewManager::getLayerType(const char* layerType) {
	if (std::strcmp(layerType, "class osgEarth::GDALImageLayer") == 0) {
		return GDALImageLayer;
	}
	else if (std::strcmp(layerType, "class osgEarth::GDALElevationLayer") == 0) {
		return GDALElevationLayer;
	}
	else if (std::strcmp(layerType, "class osgEarth::OGRFeatureSource") == 0) {
		return OGRFeatureSource;
	}
	else if (std::strcmp(layerType, "class osgEarth::FeatureImageLayer") == 0) {
		return FeatureImageLayer;
	}
	else {
		return UnknownLayer;
	}
}

void OsgTreeViewManager::reloadTree() {
	auto layers = g_osgEarthManager->getLayers();
	rootNode->removeRows(0, rootNode->rowCount());
	for (auto layer : layers) {
		auto item = new QStandardItem(layer->getName().c_str());
		item->setFlags(Qt::ItemIsEnabled);
		switch (getLayerType(layer.get()->getTypeName()))
		{
		case GDALImageLayer:
			item->setIcon(g_mediaManager->getIcon("GDALImageLayer"));
			break;
		case GDALElevationLayer:
			item->setIcon(g_mediaManager->getIcon("GDALElevationLayer"));
			break;
		case OGRFeatureSource:
			item->setIcon(g_mediaManager->getIcon("OGRFeatureSource"));
			break;
		case FeatureImageLayer:
			item->setIcon(g_mediaManager->getIcon("FeatureImageLayer"));
			break;
		default:
			item->setIcon(g_mediaManager->getIcon("File"));
			break;
		}
		rootNode->appendRow(item);
		qDebug() << layer.get()->getTypeName();
	}
}