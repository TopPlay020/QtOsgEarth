#pragma once
#include <string>
#include <unordered_map>
#include <sqlite3.h>
#include <osgEarth/FeatureImageLayer>
#include <osgEarth/AnnotationLayer>


class OsgSqlLiteManager {
public:
	using HashMap = std::unordered_map<std::string, std::string>;

	osgEarth::LayerVector loadSqlFile(const std::string& fileSrc);
	void saveSqlFile(const std::string& fileSrc);

private:
	sqlite3* db = nullptr;

	void connectToDatabase(const std::string& fileSrc);
	void disconnectDatabase();
	void executeSQL(const std::string& sql);


	void saveGDALLayer(const std::string& layerTypeName, const std::string& layerName, const std::string& layerURI, const std::string& options);
	void saveFeatureImageLayer(osgEarth::FeatureImageLayer* featureImageLayer);
	void saveAnnotationLayer(osgEarth::AnnotationLayer* annotationLayer, int order);


	std::string getColorString(const osgEarth::Color& color);
	osgEarth::Color parseColor(const std::string& colorString);
};
