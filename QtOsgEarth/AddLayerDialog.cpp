#include "AddLayerDialog.h"

AddLayerDialog::AddLayerDialog(QWidget* parent) : QDialog(parent) {
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QHBoxLayout* topLayout = new QHBoxLayout(this);

	// Left: Image label
	imageLabel = new QLabel(this);
	imageLabel->setAlignment(Qt::AlignCenter);
	imageLabel->setFixedSize(128, 128);
	imageLabel->setStyleSheet("border: 1px solid #ddd;");  // Optional border for visibility
	topLayout->addWidget(imageLabel);

	// Right: Layer type dropdown and file input button
	QVBoxLayout* rightLayout = new QVBoxLayout();

	nameEdit = new QLineEdit(this);
	nameEdit->setPlaceholderText("Enter Label Name");
	nameEdit->setStyleSheet("QLineEdit {"
		"border: 2px solid #55b5ff;"  // Green border
		"border-radius: 5px;"
		"padding: 5px;"
		"font-size: 14px;"
		"}");
	rightLayout->addWidget(nameEdit);

	// Layer type dropdown
	layerTypeCombo = new QComboBox(this);
	layerTypeCombo->addItems({ "GDALImageLayer", "GDALElevationLayer" });
	onLayerTypeSelect(0);
	connect(layerTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AddLayerDialog::onLayerTypeSelect);
	layerTypeCombo->setStyleSheet(
		"QComboBox {"
		"border-style:none;"
		"background-color: #55b5ff;"
		"color: white;"
		"border: none;"
		"border-radius: 5px;"
		"padding: 8px;"
		"font-size: 14px;"
		"outline: none;"
		"}"
		"QComboBox::drop-down {"
		"border: none;"
		"border-radius: 5px;"
		"}"
		"QComboBox::down-arrow {"
		"image: url(icons/down-arrow.png);"
		"width: 128px;"
		"height: 12px;"
		"}"
		"QComboBox QAbstractItemView {"
		"background-color: #55b5ff;"
		"color: white;"
		"border-radius: 5px;"
		"}"
		"QComboBox QAbstractItemView::item {"
		"border-radius: 5px;"
		"padding: 5px;"
		"outline: none;"
		"}"
		"QComboBox QAbstractItemView::item:selected {"
		"background-color: #4cae4f;"
		"color: white;"
		"}"
	);


	rightLayout->addWidget(layerTypeCombo);

	// File input button
	fileButton = new QPushButton("Select Data File", this);
	fileButton->setCursor(Qt::PointingHandCursor);
	fileButton->setStyleSheet(
		"QPushButton {"
		"background-color: #55b5ff;"
		"color: white;"
		"border: none;"
		"border-radius: 5px;"
		"padding: 8px;"
		"font-size: 14px;"
		"}"
		"QPushButton:hover {"
		"background-color: #4cae4f;"
		"}"
	);
	connect(fileButton, &QPushButton::clicked, this, &AddLayerDialog::selectFile);
	rightLayout->addWidget(fileButton);

	topLayout->addLayout(rightLayout);

	// Bottom layout: Add Layer button
	QHBoxLayout* bottomLayout = new QHBoxLayout();
	QPushButton* addLayerButton = new QPushButton("Add Layer", this);
	addLayerButton->setCursor(Qt::PointingHandCursor);
	addLayerButton->setStyleSheet(
		"QPushButton {"
		"background-color: #55b5ff;"
		"color: white;"
		"border: none;"
		"border-radius: 5px;"
		"padding: 8px;"
		"font-size: 14px;"
		"}"
		"QPushButton:hover {"
		"background-color: #4cae4f;"
		"}"
	);
	connect(addLayerButton, &QPushButton::clicked, this, &AddLayerDialog::accept);
	bottomLayout->addWidget(addLayerButton);

	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
}

void AddLayerDialog::onLayerTypeSelect(int index) {
	auto typeName = layerTypeCombo->itemText(index);
	layerTypeBaseName = "class osgEarth::"+typeName;
	imageLabel->setPixmap(g_mediaManager->getIcon(typeName).pixmap(128, 128));
}

int AddLayerDialog::execDialog()
{
	setFixedSize(400, 200);
	return exec(); // Execute the dialog
}

void AddLayerDialog::selectFile() {
	QString defaultPath = "D:/Tools_GIS/dataGIS/";
	if (!QDir(defaultPath).exists()) {
		defaultPath = "";  // Fallback to default location if the directory doesn't exist
	}
	QString fileName = QFileDialog::getOpenFileName(this, "Select Data File", defaultPath, "All Files (*.*)");
	if (!fileName.isEmpty()) {
		QFileInfo fileInfo(fileName);
		fileButton->setText(fileInfo.fileName());
		fileData = fileName;
	}
}

QString AddLayerDialog::getDataFile() {
	return fileData;
}

QString AddLayerDialog::getLayerName() {
	return nameEdit->text();
}

OsgTreeViewManager::LayerType AddLayerDialog::getLayerType() {
	return g_osgTreeViewManager->getLayerType(layerTypeBaseName.toUtf8());
}
