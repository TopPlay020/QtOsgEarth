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


	enum LayerType {
		GDALImageLayer,
		GDALElevationLayer,
		FeatureImageLayer,
		OGRFeatureSource,
		UnknownLayer
	};
	LayerType getLayerType(const char* layerType);

private:
	void setupView();
	void reloadTree();

	QStandardItem* rootNode;
};
