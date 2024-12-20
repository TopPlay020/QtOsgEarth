#include "globals.h"
#include <osgEarth/ElevationLayer>
#include <osgEarth/ImageLayer>
#include <osgEarth/AnnotationLayer>
#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <osgEarth/OGRFeatureSource>
#include <osgEarth/FeatureImageLayer>
#include <osgEarth/FeatureModelLayer>


void OsgTreeViewManager::createTreeView() {
	setupView();
	layerTypeToNodeMap = QMap<LayerType, QStandardItem*>();
	nodeToLayerMap = QMap<QStandardItem*, osgEarth::Layer*>();
	layerToNodeMap = QMap<osgEarth::Layer*, QStandardItem*>();

	g_osgEarthManager->addFileLoaderListener(this);
	g_osgEarthManager->addLayerEventListener(this);
}

void OsgTreeViewManager::setupView() {
	this->setHeaderHidden(true);
	this->setDragEnabled(true);
	this->setAcceptDrops(true);
	this->setDropIndicatorShown(true);
	this->setDefaultDropAction(Qt::MoveAction);

	// Create a standard item model
	model = new QStandardItemModel;
	connect(model, &QStandardItemModel::dataChanged, this, &OsgTreeViewManager::onItemChanged);

	// Create root node
	auto rootItem = model->invisibleRootItem();

	// Create nodes
	rootNode = new QStandardItem("Layers");

	// Set icons for each item
	rootNode->setIcon(g_mediaManager->getIcon("globe-earth"));

	// Add nodes to the root
	rootItem->appendRow(rootNode); // Add Root node

	// Make items unselectable
	rootNode->setFlags(Qt::ItemIsEnabled);

	this->setModel(model);

	QFont font = this->font();
	font.setPointSize(11);
	this->setFont(font);
	QSize iconSize(20, 20);
	this->setIconSize(iconSize);

	// Apply styles
	this->setStyleSheet(
		"QTreeView {"
		"    border: 0px;" // Border for the tree view
		"    selection-background-color: #a4c8f0;" // Selection color
		"    selection-color: #fff;" // Selection text color
		"    outline: 0;"
		"}"
		"QTreeView::item {"
		"    padding: 3px;" // Padding for items
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
	if (std::strcmp(layerType, "class osgEarth::GDALImageLayer") == 0)  return GDALImageLayer;
	else if (std::strcmp(layerType, "class osgEarth::GDALElevationLayer") == 0)  return GDALElevationLayer;
	else if (std::strcmp(layerType, "class osgEarth::OGRFeatureSource") == 0)  return OGRFeatureSource;
	else if (std::strcmp(layerType, "class osgEarth::FeatureImageLayer") == 0)  return FeatureImageLayer;
	else if (std::strcmp(layerType, "class osgEarth::FeatureModelLayer") == 0)  return FeatureModelLayer;
	else if (std::strcmp(layerType, "class osgEarth::AnnotationLayer") == 0)  return AnnotationLayer;
	else  return UnknownLayer;
}

QString OsgTreeViewManager::getLayerTypeName(LayerType layerType) {
	switch (layerType) {
	case OsgTreeViewManager::GDALImageLayer:	return "GDALImageLayer";
	case OsgTreeViewManager::GDALElevationLayer:return "GDALElevationLayer";
	case OsgTreeViewManager::FeatureImageLayer: return "FeatureImageLayer";
	case OsgTreeViewManager::OGRFeatureSource:	return "OGRFeatureSource";
	case OsgTreeViewManager::FeatureModelLayer: return "FeatureModelLayer";
	case OsgTreeViewManager::AnnotationLayer:	return "AnnotationLayer";
	case OsgTreeViewManager::UnknownLayer:		return "File";
	}
}

void OsgTreeViewManager::reloadTree() {
	layerTypeToNodeMap.clear();
	nodeToLayerMap.clear();
	layerToNodeMap.clear();

	auto layers = g_osgEarthManager->getLayers();
	rootNode->removeRows(0, rootNode->rowCount());
	for (auto layer : layers) {
		onLayerAdd(layer);
	}
	setExpanded(rootNode->index(), true);
}

void OsgTreeViewManager::onItemChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
	if (roles.contains(Qt::CheckStateRole)) {
		auto item = model->itemFromIndex(topLeft);
		auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(nodeToLayerMap[item]);
		auto annotationLayer = dynamic_cast<osgEarth::AnnotationLayer*>(nodeToLayerMap[item]);

		if (item->checkState() == Qt::Checked) {
			if (annotationLayer)
				annotationLayer->getNode()->setNodeMask(~0);
			if (visibleLayer)
				visibleLayer->setVisible(true);
		}
		else {
			if (annotationLayer)
				annotationLayer->getNode()->setNodeMask(0);
			if (visibleLayer)
				visibleLayer->setVisible(false);
		}
	}
}

void OsgTreeViewManager::removeNode(QStandardItem* node) {
	auto layer = nodeToLayerMap[node];
	if (layer == nullptr && node != rootNode) {
		auto nodesToDelete = QList<QStandardItem*>();
		for (int row = 0; row < node->rowCount();++row)
			nodesToDelete.append(node->child(row));
		for (auto nodeToDelete : nodesToDelete)
			removeNode(nodeToDelete);
	}
	else
		g_osgEarthManager->removeLayer(layer);
}

void OsgTreeViewManager::onLayerAdd(osgEarth::Layer* layer) {
	auto item = new QStandardItem(layer->getName().c_str());
	auto layerType = getLayerType(layer->getTypeName());
	auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);

	if (!layerTypeToNodeMap.contains(layerType) || layerTypeToNodeMap[layerType] == nullptr) {
		auto newLayerNode = new QStandardItem(getLayerTypeName(layerType));
		newLayerNode->setIcon(g_mediaManager->getIcon(getLayerTypeName(layerType)));
		newLayerNode->setFlags(Qt::ItemIsEnabled);


		rootNode->appendRow(newLayerNode);
		layerTypeToNodeMap[layerType] = newLayerNode;
	}

	auto layerNode = layerTypeToNodeMap[layerType];
	item->setIcon(g_mediaManager->getIcon(getLayerTypeName(layerType)));

	if (visibleLayer)
	{
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
		item->setData(Qt::Checked, Qt::CheckStateRole);

	}

	nodeToLayerMap[item] = layer;
	layerToNodeMap[layer] = item;
	layerNode->appendRow(item);
}

void OsgTreeViewManager::onLayerRemove(osgEarth::Layer* layer) {
	qDebug() << layer->getName();
	auto layerType = getLayerType(layer->getTypeName());
	auto layerRootNode = layerTypeToNodeMap[layerType];
	auto layerNode = layerToNodeMap[layer];

	layerRootNode->removeRow(layerNode->row());
	layerToNodeMap[layer] = nullptr;


	if (layerRootNode->rowCount() == 0) {
		rootNode->removeRow(layerRootNode->row());
		layerTypeToNodeMap[layerType] = nullptr;
	}
}

void OsgTreeViewManager::dragMoveEvent(QDragMoveEvent* event) {
	QModelIndex index = indexAt(event->pos());
	if (index.isValid()) {
		qDebug() << "Dragging over row" << index.row() << "column" << index.column();
	}
	QTreeView::dragMoveEvent(event);
}

void OsgTreeViewManager::dragEnterEvent(QDragEnterEvent* event) {
	qDebug() << "Dragging started";
	QTreeView::dragEnterEvent(event);
}

void OsgTreeViewManager::dropEvent(QDropEvent* event) {
	qDebug() << "Dropping at position" << event->pos();
	QTreeView::dropEvent(event);
}
