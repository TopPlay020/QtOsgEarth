#pragma once

#include "globals.h"
#include "GlobalEventListener.h"
#include <QMenu>
#include <QObject>
#include <osg/Vec3d>
#include <OsgEarthUtility.h>

class OsgEarthContextMenu : public QObject, public GlobalEventListener , public UiUpdateAble {
public:
	void createContextMenu();

	void onContextMenu() override;
	void onMouseLeftRelese() override;

	void updateUi() override;
private:

	QMenu* viewerContextMenu;

	QAction* addLabelAction;

	QMenu* addDistanceMenu;
	QAction* euclideanDistanceAction;
	QAction* linearInterpolationDistanceAction;

	QAction* copyPositionAction;

	osg::Vec3d contextMenuPoint;


	bool isCommandInWork = false;
	bool isDistanceAction = false;
	bool isEuclideanDistanceAction = false;
	osg::Vec3d firstPointDistance = osg::Vec3d();
	osg::Vec3d lastPointDistance = osg::Vec3d();
	OsgLine distanceCommandUnit = nullptr;

private slots:
	void onAddLabelAction();
	void onCopyPositionAction();
	void onDistanceAction();
};
