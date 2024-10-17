#pragma once
#include <osgEarth/PlaceNode>
#include <osgUtil/LineSegmentIntersector>
#include <osgEarth/AnnotationLayer>
#include <osgEarth/Feature>
#include <osgEarth/FeatureNode>
#include <qstring.h>

class OsgNodeAdapter {
public:
	virtual osg::Node* getNode() = 0;

};

class OsgLocationLabel {
public:
	OsgLocationLabel() {
		layer = new osgEarth::AnnotationLayer();
		node = new osgEarth::PlaceNode();
		layer->addChild(node);
	}
	void sePosition(osgEarth::GeoPoint position) {
		node->setPosition(position);
	}
	void setText(std::string text) {
		if (text.empty())
			text = "Empty";
		node->setText(text);
	}
	void setName(std::string name) {
		if (name.empty())
			name = "Label";
		node->setName(name);
		layer->setName(name);
	}
	void setTextColor(QColor color) {
		osgEarth::Style style = node->getStyle();
		osgEarth::TextSymbol* textSymbol = style.getOrCreate<osgEarth::TextSymbol>();
		osgEarth::Color osgColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
		textSymbol->fill()->color() = osgColor;
		node->setStyle(style);
	}
	void setIcon(std::string url , float iconSize) {
		osgEarth::Style style = node->getStyle();
		osgEarth::IconSymbol* icon = style.getOrCreate<osgEarth::IconSymbol>();
		icon->url()->setLiteral(url);
		icon->declutter() = true; 
		icon->scale() = iconSize;
		node->setStyle(style);
	}
	void update() {
		node->dirty();
	}

	osgEarth::Layer* getLayer() {
		return layer;
	}
private:
	osgEarth::PlaceNode* node;
	osgEarth::AnnotationLayer* layer;
};

class OsgLabel : public OsgNodeAdapter {
public:
	OsgLabel(osgEarth::PlaceNode* node, osgEarth::AnnotationLayer* layer) : node(node), layer(layer) {};
	void setText(QString text) {
		node->setText(text.toStdString());
		layer->setName(text.toStdString());
		update();
	};
	osg::Node* getNode() override { return dynamic_cast<osg::Node*>(node); };
private:
	osgEarth::PlaceNode* node;
	osgEarth::AnnotationLayer* layer;

	void update() {
		auto parent = node->getParent(0);
		auto geoPoint = node->getPosition();
		auto style = node->getStyle();
		auto text = node->getText();

		parent->removeChild(node);
		node = new osgEarth::PlaceNode(geoPoint, text, style);
		parent->addChild(node);
	}
};

class OsgLine : public OsgNodeAdapter {
public:
	OsgLine(osgEarth::FeatureNode* featureNode) : featureNode(featureNode) {};
	osg::Node* getNode() override { return dynamic_cast<osg::Node*>(featureNode); };
	void setEndPoint(osg::Vec3d newEndPoint) {

		auto feature = featureNode->getFeature();
		auto line = dynamic_cast<osgEarth::LineString*>(feature->getGeometry());
		auto mapNode = dynamic_cast<osgEarth::MapNode*>(featureNode->getParent(0));

		// Create a new GeoPoint for the new end point
		osgEarth::GeoPoint newGeoPointEnd(mapNode->getMapSRS(), newEndPoint.x(), newEndPoint.y(), newEndPoint.z(), osgEarth::ALTMODE_ABSOLUTE);

		// Update the line's end point
		line->back() = newGeoPointEnd.vec3d(); // Change the last point in the line
		featureNode->dirty();
	}

private:
	osgEarth::FeatureNode* featureNode;

};

class OsgPolygon : public OsgNodeAdapter {
public:
	OsgPolygon(osgEarth::FeatureNode* featureNode) : featureNode(featureNode) {};
	osg::Node* getNode() override { return dynamic_cast<osg::Node*>(featureNode); };
private:
	osgEarth::FeatureNode* featureNode;
};