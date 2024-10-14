#pragma once

#include "globals.h"
#include "GlobalEventListener.h"
#include <QObject>


class OsgTreeViewContextMenu : public QObject {
public:
	void createContextMenu();

private:
	void onContextMenu(const QPoint& pos);

	QMenu* contextMenu;

	QAction* changeLayerNameAction;
	QAction* removeLayerAction;

	QStandardItem* selectedItem;

private slots:
	void onChangeLayerNameAction();
	void onRemoveLayerAction();
};