#include "globals.h"
#include <osgEarth/ElevationLayer>
#include <osgEarth/ImageLayer>

void OsgTreeViewManager::createTreeView() {
	g_osgEarthManager->addFileLoaderListener(this);
	setupView();
	layerTreeNodes = QMap<LayerType, QStandardItem*>();
	layerNodes = QMap<QStandardItem*, osgEarth::Layer*>();
}

void OsgTreeViewManager::setupView() {
	this->setHeaderHidden(true);

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
		"    border: 1px solid #999;" // Border for the tree view
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

QString OsgTreeViewManager::getLayerTypeName(LayerType layerType) {
	switch (layerType)
	{
	case OsgTreeViewManager::GDALImageLayer:
		return "GDALImageLayer";
	case OsgTreeViewManager::GDALElevationLayer:
		return "GDALElevationLayer";
	case OsgTreeViewManager::FeatureImageLayer:
		return "FeatureImageLayer";
	case OsgTreeViewManager::OGRFeatureSource:
		return "OGRFeatureSource";
	case OsgTreeViewManager::UnknownLayer:
		return "File";
	}
}

void OsgTreeViewManager::reloadTree() {
	auto layers = g_osgEarthManager->getLayers();
	rootNode->removeRows(0, rootNode->rowCount());
	for (auto layer : layers) {
		auto item = new QStandardItem(layer->getName().c_str());
		auto layerType = getLayerType(layer.get()->getTypeName());
		auto imageLayer = dynamic_cast<osgEarth::ImageLayer*>(layer.get());

		if (!layerTreeNodes.contains(layerType)) {
			auto newLayerNode = new QStandardItem(getLayerTypeName(layerType));
			newLayerNode->setIcon(g_mediaManager->getIcon(getLayerTypeName(layerType)));
			newLayerNode->setFlags(Qt::ItemIsEnabled);


			rootNode->appendRow(newLayerNode);
			layerTreeNodes[layerType] = newLayerNode;
		}

		auto layerNode = layerTreeNodes[layerType];
		item->setIcon(g_mediaManager->getIcon(getLayerTypeName(layerType)));

		if (imageLayer)
		{
			item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
			item->setData(Qt::Checked, Qt::CheckStateRole);

		}
		/*connect(model, &QStandardItemModel::dataChanged, [](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
			if (roles.contains(Qt::CheckStateRole)) {
				QStandardItem* item = static_cast<QStandardItemModel*>(topLeft.model())->itemFromIndex(topLeft);
				if (item->checkState() == Qt::Checked) {
					qDebug() << "Item checked:" << item->text();
				}
				else {
					qDebug() << "Item unchecked:" << item->text();
				}
			}
			});*/

		layerNodes[item] = layer.get();
		layerNode->appendRow(item);
		qDebug() << layer.get()->getTypeName();
	}
}
void OsgTreeViewManager::onItemChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
	if (roles.contains(Qt::CheckStateRole)) {
		auto item = model->itemFromIndex(topLeft);
		auto imageLayer = dynamic_cast<osgEarth::ImageLayer*>(layerNodes[item]);
		if (item->checkState() == Qt::Checked) {
			if (imageLayer)
				imageLayer->setVisible(true);
		}
		else {
			if (imageLayer)
				imageLayer->setVisible(false);
		}
	}
}