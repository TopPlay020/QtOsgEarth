#pragma once
#include "globals.h"
#include <QHBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>

class OsgTreeViewManager : public QTreeView, public FileLoaderListener
{
public:
	void createTreeView();


	void onFileLoadingEnd(const QString& fileName, bool success) { onFileReloadingEnd(success); };
	void onFileReloadingEnd(bool success) { reloadTree(); };

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
	LayerType getLayerType(const char* layerType);
	QString getLayerTypeName(LayerType layerType);

private:
	void setupView();

	QStandardItemModel* model;
	QStandardItem* rootNode;
	QMap<LayerType, QStandardItem*> layerTreeNodes;
	QMap<QStandardItem*, osgEarth::Layer*> layerNodes;

private slots:
	void onItemChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
};
