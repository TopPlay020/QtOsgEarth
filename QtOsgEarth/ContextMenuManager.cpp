#include <globals.h>
#include <QClipboard>
#include <QApplication>
#include <QInputDialog>
#include <DistanceUtility.h>

void ContextMenuManager::createContextMenu() {
	viewerContextMenu = new QMenu(g_mainWindow);

	addLabelAction = new QAction("&add Label", g_mainWindow);
	addLabelAction->setIcon(g_mediaManager->getIcon("Label"));
	QObject::connect(addLabelAction, &QAction::triggered, this, &ContextMenuManager::onAddLabelAction);
	viewerContextMenu->addAction(addLabelAction);


	addDistanceMenu = new QMenu("Add Distance", g_mainWindow);
	addDistanceMenu->setIcon(g_mediaManager->getIcon("Distance"));

	euclideanDistanceAction = new QAction("&Euclidean Distance", g_mainWindow);
	euclideanDistanceAction->setIcon(g_mediaManager->getIcon("Math"));
	QObject::connect(euclideanDistanceAction, &QAction::triggered, this, [this] {
		isEuclideanDistanceAction = true;
		onDistanceAction();
		});


	linearInterpolationDistanceAction = new QAction("&Linear Interpolation Distance", g_mainWindow);
	linearInterpolationDistanceAction->setIcon(g_mediaManager->getIcon("Math"));
	QObject::connect(linearInterpolationDistanceAction, &QAction::triggered, this, [this] {
		isEuclideanDistanceAction = false;
		onDistanceAction();
		});

	addDistanceMenu->addAction(euclideanDistanceAction);
	addDistanceMenu->addAction(linearInterpolationDistanceAction);

	viewerContextMenu->addMenu(addDistanceMenu);

	viewerContextMenu->addSeparator();

	copyPositionAction = new QAction("&Copy Position", g_mainWindow);
	copyPositionAction->setIcon(g_mediaManager->getIcon("Copy"));

	QObject::connect(copyPositionAction, &QAction::triggered, this, &ContextMenuManager::onCopyPositionAction);

	viewerContextMenu->addAction(copyPositionAction);

	g_globalEventProvider->addMouseEventListener(this);
	g_mainWindow->addUpdateAble(this);
}

void ContextMenuManager::onContextMenu() {
	QPoint globalMousePos = QCursor::pos();
	QPoint localMousePos = g_osgEarthManager->mapFromGlobal(globalMousePos);

	int mouseX = localMousePos.x();
	int mouseY = localMousePos.y();

	contextMenuPoint = g_osgEarthManager->getMouseCoordinatesInGeoPoint(mouseX, mouseY);
	copyPositionAction->setText(QString("%1, %2, %3").arg(contextMenuPoint.x()).arg(contextMenuPoint.y()).arg(contextMenuPoint.z()));

	QPoint globalPos = QCursor::pos();  // Get global mouse position
	viewerContextMenu->exec(globalPos); // Show the menu at the top-left of the mouse position
}

void ContextMenuManager::onAddLabelAction() {
	OsgLabel label = g_osgEarthManager->addLabel(contextMenuPoint, "Label");
	bool ok; // to check if the user pressed OK
	QString labelName = QInputDialog::getText(nullptr,
		"Input Label Name",
		"Please enter Label name:",
		QLineEdit::Normal,
		QString(),
		&ok);
	if (ok)
		if (!labelName.isEmpty())
			label.setText(labelName);
		else
			label.setText("Empty Label");
	else
		g_osgEarthManager->removeNode(label);

}

void ContextMenuManager::onCopyPositionAction() {
	QString actionText = copyPositionAction->text();
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setText(actionText);
}


void ContextMenuManager::onDistanceAction() {
	if (!isCommandInWork) {
		//Step 01
		isCommandInWork = true;
		isDistanceAction = true;

		firstPointDistance = contextMenuPoint;

		QPoint globalMousePos = QCursor::pos();
		QPoint localMousePos = g_osgEarthManager->mapFromGlobal(globalMousePos);
		int mouseX = localMousePos.x();
		int mouseY = localMousePos.y();
		auto currentPoint = g_osgEarthManager->getMouseCoordinatesInGeoPoint(mouseX, mouseY);

		distanceCommandUnit = g_osgEarthManager->addLine(firstPointDistance, currentPoint);
	}
	else if (isDistanceAction) {
		//Step 02
		double distance = haversineDistance(firstPointDistance, lastPointDistance);

		if (!isEuclideanDistanceAction) {
			double SEGMENT_LENGTH = 30.0;
			int numSegments = distance / SEGMENT_LENGTH;

			auto segmentsList = QList<osg::Vec3d>();
			for (int i = 0; i <= numSegments; ++i) {
				double fraction = static_cast<double>(i) / numSegments;       
				auto segmentPoint = firstPointDistance * (1.0 - fraction) + lastPointDistance * fraction;
				segmentsList.append(segmentPoint);
			}

			g_osgEarthManager->adjustAltitudes(segmentsList);

			distance = 0.0;
			for (int i = 0; i < numSegments; ++i) {
				distance += haversineDistance(segmentsList[i], segmentsList[i+1]);
			}

			g_osgEarthManager->addPolygon(segmentsList);
			g_osgEarthManager->removeNode(distanceCommandUnit);
		}


		auto middlePoint = (firstPointDistance + lastPointDistance) / 2;
		g_osgEarthManager->addLabel(middlePoint, QString("%1 Km").arg(QString::number(distance / 1000.0, 'f', 2)));

		isEuclideanDistanceAction = false;
		isCommandInWork = false;
		distanceCommandUnit = nullptr;
	}

}


void ContextMenuManager::updateUi() {
	if (isCommandInWork) {
		if (isDistanceAction) {
			QPoint globalMousePos = QCursor::pos();
			QPoint localMousePos = g_osgEarthManager->mapFromGlobal(globalMousePos);
			int mouseX = localMousePos.x();
			int mouseY = localMousePos.y();
			lastPointDistance = g_osgEarthManager->getMouseCoordinatesInGeoPoint(mouseX, mouseY);

			distanceCommandUnit.setEndPoint(lastPointDistance);
		}
	}
}

void ContextMenuManager::onMouseLeftRelese() {
	if (isCommandInWork) {
		if (isDistanceAction) {
			onDistanceAction();
		}
	}
}