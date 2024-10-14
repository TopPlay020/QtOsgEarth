#include <globals.h>
#include <OsgTreeViewContextMenu.h>
#include <QInputDialog>


void OsgTreeViewContextMenu::createContextMenu() {
	contextMenu = new QMenu(g_mainWindow);

	changeLayerNameAction = new QAction("&change Layer Name", g_mainWindow);
	changeLayerNameAction->setIcon(g_mediaManager->getIcon("layerName"));
	QObject::connect(changeLayerNameAction, &QAction::triggered, this, &OsgTreeViewContextMenu::onChangeLayerNameAction);
	contextMenu->addAction(changeLayerNameAction);

	removeLayerAction = new QAction("&remove Layer", g_mainWindow);
	removeLayerAction->setIcon(g_mediaManager->getIcon("layerRemove"));
	QObject::connect(removeLayerAction, &QAction::triggered, this, &OsgTreeViewContextMenu::onRemoveLayerAction);
	contextMenu->addAction(removeLayerAction);

	g_osgTreeViewManager->setContextMenuPolicy(Qt::CustomContextMenu);
	QObject::connect(g_osgTreeViewManager, &QTreeView::customContextMenuRequested, this, &OsgTreeViewContextMenu::onContextMenu);
}


void OsgTreeViewContextMenu::onContextMenu(const QPoint& pos) {
	QModelIndex index = g_osgTreeViewManager->indexAt(pos);
	if (!index.isValid() || g_osgTreeViewManager->model->index(0,0) == index) return;

	//I need to make model in OsgTreeViewManager provate and add method to use itemFromIndex !!
	selectedItem = g_osgTreeViewManager->model->itemFromIndex(index);
	if (!selectedItem) return;

	// Change the selectedItem selected background
	QBrush oldBrush = selectedItem->background();
	QColor skyBlue(135, 206, 235);
	QBrush newBrush(skyBlue);
	selectedItem->setBackground(newBrush);


	contextMenu->exec(g_osgTreeViewManager->mapToGlobal(pos));

	// Restore the selectedItem selected background
	if(selectedItem)
		selectedItem->setBackground(oldBrush);

}

void OsgTreeViewContextMenu::onChangeLayerNameAction() {
	auto layer = g_osgTreeViewManager->getLayerFromQStandardItem(selectedItem);
	bool ok; // to check if the user pressed OK
	QString layerName = QInputDialog::getText(nullptr,
		"Input new Layer Name",
		"Please enter new Layer Name:",
		QLineEdit::Normal,
		QString(),
		&ok);
	if (ok && !layerName.isEmpty()) {
		layer->setName(layerName.toStdString());
		selectedItem->setText(layerName);
	}

}

void OsgTreeViewContextMenu::onRemoveLayerAction() {
	g_osgTreeViewManager->removeNode(selectedItem);
	selectedItem = nullptr;
}