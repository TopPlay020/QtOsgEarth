#pragma once

#include "globals.h"
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>

class AddLayerDialog : public QDialog {
	Q_OBJECT
public:
	explicit AddLayerDialog(QWidget* parent = nullptr);
	void onLayerTypeSelect(int index);
	int execDialog();

	QString getDataFile();
	QString getLayerName();
	OsgTreeViewManager::LayerType getLayerType();
private slots:
	void selectFile();

private:
	QLabel* imageLabel;
	QComboBox* layerTypeCombo;
	QPushButton* fileButton;
	QLineEdit* nameEdit;

	QString fileData = nullptr;
	QString layerTypeBaseName;
};