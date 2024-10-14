#pragma once
#include "globals.h"
#include "LayerEventListener.h"
#include <QHBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>

class OsgTreeViewManager : public QTreeView, public FileLoaderListener , public LayerEventListener
{
public:
	void createTreeView();


	void onFileLoadingEnd(const QString& fileName, bool success) { onFileReloadingEnd(success); };
	void onFileReloadingEnd(bool success) { reloadTree(); };

	void onLayerAdd(osgEarth::Layer* layer) override;
	void onLayerUpdate(osgEarth::Layer* layer) override {};
	void onLayerRemove(osgEarth::Layer* layer) override;

	void reloadTree();

	enum LayerType {
		GDALImageLayer,
		GDALElevationLayer,
		FeatureImageLayer,
		OGRFeatureSource,
		FeatureModelLayer,
		AnnotationLayer,
		UnknownLayer
	};

	LayerType getLayerType(osgEarth::Layer* layer);
	//LayerType getLayerType(const char* layerType);
	QString getLayerTypeName(LayerType layerType);

	QStandardItemModel* model;
private:
	void setupView();

	QStandardItem* rootNode;
	QMap<LayerType, QStandardItem*> layerTypeToNodeMap;
	QMap<osgEarth::Layer*, QStandardItem*> layerToNodeMap;
	QMap<QStandardItem*, osgEarth::Layer*> nodeToLayerMap;
public:
	osgEarth::Layer* getLayerFromQStandardItem(QStandardItem* item) { return nodeToLayerMap[item]; };
private slots:
	void onItemChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
};
