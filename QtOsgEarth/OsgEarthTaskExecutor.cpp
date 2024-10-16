#include<globals.h>
#include <AddLocationDialog.h>

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
	locationLabel->setIcon(dialog->getIconUrl().toStdString());

	g_osgEarthManager->addLayer(locationLabel->getLayer());
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