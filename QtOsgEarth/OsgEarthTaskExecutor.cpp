#include<globals.h>
#include <osgEarth\GDAL>
#include <AddLocationDialog.h>
#include <AddLayerDialog.h>

OsgEarthTaskExecutor::OsgEarthTaskExecutor() {
	g_mainWindow->addUpdateAble(this);
	g_globalEventProvider->addMouseEventListener(this);
}

void OsgEarthTaskExecutor::addLabelTask() {
	AddLocationDialog* dialog = new AddLocationDialog(g_mainWindow);
	auto result = dialog->execDialog();

	if (result == QDialog::Rejected)
		return;

	isAddLabelTaskRunning = true;

	locationLabel = new OsgLocationLabel();
	locationLabel->setName(dialog->getName().toStdString());
	locationLabel->setText(dialog->getText().toStdString());
	locationLabel->setTextColor(dialog->getTextColor());
	locationLabel->setIcon(dialog->getIconUrl().toStdString(), dialog->getIconSize());

	g_osgEarthManager->addLayer(locationLabel->getLayer());
}

void OsgEarthTaskExecutor::addLayerTask()
{
	AddLayerDialog* addLayerDialog = new AddLayerDialog(g_mainWindow);
	auto result = addLayerDialog->execDialog();

	if (result == QDialog::Rejected || addLayerDialog->getDataFile().isEmpty())
		return;

	switch (addLayerDialog->getLayerType()) {
	case OsgTreeViewManager::GDALImageLayer: {
		auto gdalImageLayer = new osgEarth::GDALImageLayer();
		gdalImageLayer->setName(addLayerDialog->getLayerName().toStdString());
		gdalImageLayer->setURL(osgEarth::URI(addLayerDialog->getDataFile().toStdString()));
		g_osgEarthManager->addLayer(gdalImageLayer);
		break;
	}
	case OsgTreeViewManager::GDALElevationLayer: {
		auto gdalElevationLayer = new osgEarth::GDALElevationLayer();
		gdalElevationLayer->setName(addLayerDialog->getLayerName().toStdString());
		gdalElevationLayer->setURL(osgEarth::URI(addLayerDialog->getDataFile().toStdString()));
		g_osgEarthManager->addLayer(gdalElevationLayer);
		break;
	}
	default:
		// Optional: Handle unexpected layer types.
		break;
	}

}

void OsgEarthTaskExecutor::onMouseClick() {
	if (isAddLabelTaskRunning) {
		isAddLabelTaskRunning = false;
		locationLabel = nullptr;
	}
}

void OsgEarthTaskExecutor::updateUi() {
	QPoint globalMousePos = QCursor::pos();
	QPoint localMousePos = g_osgEarthManager->mapFromGlobal(globalMousePos);
	int mouseX = localMousePos.x();
	int mouseY = localMousePos.y();
	auto currentMapGeoPoint = g_osgEarthManager->getMouseCoordinatesInGeoPoint(mouseX, mouseY);

	if (isAddLabelTaskRunning) {
		locationLabel->sePosition(currentMapGeoPoint);
		locationLabel->update();
	}

}