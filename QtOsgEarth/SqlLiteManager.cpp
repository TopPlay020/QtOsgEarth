#include <globals.h>
#include <format>
#include <map>
#include "OsgSqlLiteManager.h"
#include <osgEarth/GDAL>
#include <osgEarth/LabelNode>
#include <osgEarth/OGRFeatureSource>


void OsgSqlLiteManager::connectToDatabase(const std::string& fileSrc) {
	if (sqlite3_open(fileSrc.c_str(), &db) != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		db = nullptr; // Ensure db is null on failure
	}
}

void OsgSqlLiteManager::disconnectDatabase() {
	if (db) {
		sqlite3_close(db);
	}
}

void OsgSqlLiteManager::executeSQL(const std::string& sql)
{
	char* errorMessage = nullptr;
	if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
		std::cerr << "SQL error: " << errorMessage << std::endl;
		sqlite3_free(errorMessage); // Free the error message memory
	}
}


osgEarth::LayerVector OsgSqlLiteManager::loadSqlFile(const std::string& fileSrc) {
	osgEarth::LayerVector result;

	connectToDatabase(fileSrc);

	// Retrieve GDAL Image Layers
	std::string sqlSelectGDALImage = "SELECT layerName, layerURI, options FROM GDALImageLayer;";
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db, sqlSelectGDALImage.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			auto layerName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			auto layerURI = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
			auto optionsString = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

			osgEarth::Config optionsConfig;
			optionsConfig.fromJSON(optionsString);

			// Create GDALImageLayer
			osg::ref_ptr<osgEarth::GDALImageLayer> gdalLayer = new osgEarth::GDALImageLayer();
			gdalLayer->options().merge(optionsConfig);
			gdalLayer->setName(layerName);
			gdalLayer->setURL(layerURI);
			result.push_back(gdalLayer);
		}
	}

	sqlite3_finalize(stmt);

	// Retrieve GDAL Elevation Layers
	std::string sqlSelectGDALElevation = "SELECT layerName, layerURI, options FROM GDALElevationLayer;";
	if (sqlite3_prepare_v2(db, sqlSelectGDALElevation.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			auto layerName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			auto layerURI = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
			auto optionsString = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

			osgEarth::Config optionsConfig;
			optionsConfig.fromJSON(optionsString);

			// Create GDALElevationLayer
			osg::ref_ptr<osgEarth::GDALElevationLayer> elevationLayer = new osgEarth::GDALElevationLayer();
			elevationLayer->options().merge(optionsConfig);
			elevationLayer->setName(layerName);
			elevationLayer->setURL(layerURI);
			result.push_back(elevationLayer);
		}
	}
	sqlite3_finalize(stmt);

	// Retrieve FeatureImageLayer
	std::string sqlFeatureImageLayer = "SELECT layerName, featureURl, options FROM FeatureImageLayer;";
	if (sqlite3_prepare_v2(db, sqlFeatureImageLayer.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			auto layerName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			auto featureURl = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
			auto optionsString = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));


			osgEarth::Config optionsConfig;
			optionsConfig.fromJSON(optionsString);

			// Create OGRFeatureSource
			auto features = new osgEarth::OGRFeatureSource();
			features->setURL(featureURl);

			// Create GDALElevationLayer
			auto featureImageLayer = new osgEarth::FeatureImageLayer();
			featureImageLayer->setName(layerName);


			featureImageLayer->setFeatureSource(features);
			featureImageLayer->options().merge(optionsConfig);

			result.push_back(featureImageLayer);
		}
	}
	sqlite3_finalize(stmt);



	// Retrieve Annotation Layers
	std::map<int, osgEarth::AnnotationLayer*> annotationMap;
	auto sqlSelectAnnotation = "SELECT id, layer_name, options FROM AnnotationLayer;";
	if (sqlite3_prepare_v2(db, sqlSelectAnnotation, -1, &stmt, nullptr) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {

			auto layer_id = sqlite3_column_int(stmt, 0);
			auto layer_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
			auto optionsString = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));


			osgEarth::Config optionsConfig;
			optionsConfig.fromJSON(optionsString);

			auto annotationLayer = new osgEarth::AnnotationLayer();
			annotationLayer->setName(layer_name);
			annotationLayer->options().merge(optionsConfig);

			annotationMap.insert(std::make_pair(layer_id, annotationLayer));

			result.push_back(annotationLayer);
		}

	}
	sqlite3_finalize(stmt);


	// Retrieve PlaceNode LabelNode
	for (auto className : { "PlaceNode", "LabelNode" }) {

		auto sqlSelectPlaceNodes = "SELECT layer_id, label, text, longitude, latitude, altitude, style FROM " + std::string(className) + ";";
		if (sqlite3_prepare_v2(db, sqlSelectPlaceNodes.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				auto layer_id = sqlite3_column_int(stmt, 0);
				auto label = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
				auto text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
				auto longitude = sqlite3_column_double(stmt, 3);
				auto latitude = sqlite3_column_double(stmt, 4);
				auto altitude = sqlite3_column_double(stmt, 5);
				auto styleString = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

				// Create PlaceNode for each annotation
				osgEarth::GeoPositionNode* pointNode;
				if (className == "PlaceNode")
				{
					auto placeNode = new osgEarth::PlaceNode(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), longitude, latitude, altitude));
					placeNode->setText(text);
					pointNode = placeNode;
				}
				else
				{
					auto labelNode = new osgEarth::LabelNode(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), longitude, latitude, altitude));
					labelNode->setText(text);
					pointNode = labelNode;
				}

				pointNode->setName(label);

				osgEarth::Config config;
				config.fromJSON(styleString);
				auto style = osgEarth::Style(config);

				pointNode->setStyle(style);

				annotationMap.at(layer_id)->addChild(pointNode);
			}

		}
	}
	sqlite3_finalize(stmt);


	disconnectDatabase();
	return result;
}


void OsgSqlLiteManager::saveSqlFile(const std::string& fileSrc) {
	if (std::filesystem::exists(fileSrc)) {
		std::filesystem::remove(fileSrc);
	}
	connectToDatabase(fileSrc);

	auto layers = g_osgEarthManager->getLayers();
	auto annotationLayerOrder = 1;
	for (auto layer : layers) {
		auto layerName = layer->getName();
		auto layerType = g_osgTreeViewManager->getLayerType(layer->getTypeName());
		auto layerTypeName = g_osgTreeViewManager->getLayerTypeName(layerType).toStdString();
		switch (layerType)
		{
		case OsgTreeViewManager::GDALImageLayer:
		{
			auto gdalLayer = dynamic_cast<osgEarth::GDALImageLayer*>(layer.get());
			auto layerURI = gdalLayer->getURL().full();
			auto configString = gdalLayer->options().getConfig().toJSON();
			saveGDALLayer(layerTypeName, layerName, layerURI, configString);
			break;
		}
		case OsgTreeViewManager::GDALElevationLayer:
		{
			auto gdalLayer = dynamic_cast<osgEarth::GDALElevationLayer*>(layer.get());
			auto layerURI = gdalLayer->getURL().full();
			auto configString = gdalLayer->options().getConfig().toJSON();
			saveGDALLayer(layerTypeName, layerName, layerURI, configString);
			break;
		}
		case OsgTreeViewManager::FeatureImageLayer:
			saveFeatureImageLayer(dynamic_cast<osgEarth::FeatureImageLayer*>(layer.get()));
			break;
		case OsgTreeViewManager::OGRFeatureSource:
			break;
		case OsgTreeViewManager::FeatureModelLayer:
			break;
		case OsgTreeViewManager::AnnotationLayer:
			saveAnnotationLayer(dynamic_cast<osgEarth::AnnotationLayer*>(layer.get()), annotationLayerOrder++);
			break;
		}
	}

	disconnectDatabase();
}

void OsgSqlLiteManager::saveGDALLayer(const std::string& layerTypeName, const std::string& layerName, const std::string& layerURI, const std::string& options)
{
	std::string sqlCreateTable = "CREATE TABLE IF NOT EXISTS " + layerTypeName + " (layerName TEXT, layerURI TEXT, options TEXT);";
	std::string sqlInsert = "INSERT INTO " + layerTypeName + " (layerName, layerURI, options) VALUES ('" + layerName + "', '" + layerURI + "', '" + options + "');";
	executeSQL(sqlCreateTable);
	executeSQL(sqlInsert);
}

void OsgSqlLiteManager::saveFeatureImageLayer(osgEarth::FeatureImageLayer* featureImageLayer) {
	auto featureUrl = dynamic_cast<osgEarth::OGRFeatureSource*>(featureImageLayer->getFeatureSource())->getURL().full();
	auto layerName = featureImageLayer->getName();
	auto optionsString = featureImageLayer->options().getConfig().toJSON();

	std::string sqlCreateTable = "CREATE TABLE IF NOT EXISTS FeatureImageLayer (layerName TEXT, featureURl TEXT, options TEXT);";
	std::string sqlInsert = "INSERT INTO FeatureImageLayer (layerName, featureURl, options) VALUES ('" + layerName + "', '" + featureUrl + "', '" + optionsString + "');";
	executeSQL(sqlCreateTable);
	executeSQL(sqlInsert);

}

void OsgSqlLiteManager::saveAnnotationLayer(osgEarth::AnnotationLayer* annotationLayer, int layer_id) {
	// Create a table for annotations if it doesn't exist
	auto createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS AnnotationLayer (
            id INTEGER PRIMARY KEY ,
            layer_name TEXT,
			options TEXT
        );
    )";
	executeSQL(createTableSQL);

	auto layerName = annotationLayer->getName();
	auto optionsString = annotationLayer->options().getConfig().toJSON();

	auto insertSQL = "INSERT INTO AnnotationLayer (id, layer_name, options) VALUES (" + std::to_string(layer_id) + ",'" + layerName + "', '" + optionsString + "');";
	executeSQL(insertSQL);

	// Iterate through the features in the annotation layer
	std::function<void(osg::Group*)> processGroup = [&](osg::Group* currentGroup) {
		for (size_t i = 0; i < currentGroup->getNumChildren(); ++i) {
			auto child = currentGroup->getChild(i);

			if (auto geoPosNode = dynamic_cast<osgEarth::GeoPositionNode*>(child)) {
				auto nodeClassName = "";
				if (dynamic_cast<osgEarth::PlaceNode*>(child))
					nodeClassName = "PlaceNode";
				else if (dynamic_cast<osgEarth::LabelNode*>(child))
					nodeClassName = "LabelNode";

				std::stringstream createTableSQL;
				createTableSQL << R"(CREATE TABLE IF NOT EXISTS )" << nodeClassName << R"( (
					id INTEGER PRIMARY KEY AUTOINCREMENT,
					layer_id INTEGER,
					label TEXT,
					text TEXT,
					longitude REAL,
					latitude REAL,
					altitude REAL,
					style TEXT
				);)";
				executeSQL(createTableSQL.str());

				auto labelName = geoPosNode->getName();
				auto labelText = geoPosNode->getText();
				auto position = geoPosNode->getPosition();
				auto styleString = geoPosNode->getStyle().getConfig().toJSON();

				std::stringstream insertSQL;
				insertSQL << "INSERT INTO " << nodeClassName << " (layer_id, label, text, longitude, latitude, altitude, style) VALUES ('"
					<< layer_id << "', '"
					<< labelName << "','"
					<< labelText << "', "
					<< position.x() << ", "
					<< position.y() << ", "
					<< position.z() << ", '"
					<< styleString << "');";
				executeSQL(insertSQL.str());

			}
			else if (auto subgroup = dynamic_cast<osg::Group*>(child)) {
				processGroup(subgroup);
			}
		}
		};

	processGroup(annotationLayer->getGroup());
}

std::string OsgSqlLiteManager::getColorString(const osgEarth::Color& color) {
	std::ostringstream ss;
	ss << color.r() << " " << color.g() << " " << color.b() << " " << color.a();
	return ss.str();
}

osgEarth::Color OsgSqlLiteManager::parseColor(const std::string& colorString) {
	std::istringstream ss(colorString);
	float r, g, b, a;

	if (ss >> r >> g >> b >> a) {
		return osgEarth::Color(r, g, b, a);
	}
	return osgEarth::Color::Black; // default color if parsing fails
}