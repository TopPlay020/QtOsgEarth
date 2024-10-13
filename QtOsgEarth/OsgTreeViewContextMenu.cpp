#include <globals.h>
#include <OsgTreeViewContextMenu.h>

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
	QObject::connect(g_osgTreeViewManager,&QTreeView::customContextMenuRequested , this , &OsgTreeViewContextMenu::onContextMenu);
}


void OsgTreeViewContextMenu::onContextMenu(const QPoint& pos) {
	QModelIndex index = g_osgTreeViewManager->indexAt(pos);
	if (!index.isValid()) return;

	//I need to make model in OsgTreeViewManager provate and add method to use itemFromIndex !!
	QStandardItem* item = g_osgTreeViewManager->model->itemFromIndex(index);
	if (!item) return;

	// Change the item selected background
	QBrush oldBrush = item->background();
	QColor skyBlue(135, 206, 235);
	QBrush newBrush(skyBlue);
	item->setBackground(newBrush);
	

	contextMenu->exec(g_osgTreeViewManager->mapToGlobal(pos));

	// Restore the item selected background
	item->setBackground(oldBrush);
	
}

void OsgTreeViewContextMenu::onChangeLayerNameAction() {

}

void OsgTreeViewContextMenu::onRemoveLayerAction() {

}