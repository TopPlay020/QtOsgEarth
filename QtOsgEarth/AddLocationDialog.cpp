#include <globals.h>
#include "AddLocationDialog.h"
#include <QApplication>
#include <QMainWindow>
#include <QAction>
#include <QMenuBar>

AddLocationDialog::AddLocationDialog(QWidget* parent) : QDialog(parent) {
	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	// Image display label
	imageLabel = new QLabel(this);
	setDefaultImage(); // Set the default image
	imageLabel->setAlignment(Qt::AlignCenter); // Center the label
	mainLayout->addWidget(imageLabel, 0, Qt::AlignCenter); // Center the label in the main layout

	// Create a horizontal layout for name and text edits
	QHBoxLayout* hLayout = new QHBoxLayout();

	nameEdit = new QLineEdit(this);
	nameEdit->setPlaceholderText("Enter Label Name");
	hLayout->addWidget(nameEdit);

	textEdit = new QLineEdit(this);
	textEdit->setPlaceholderText("Enter Label Text");
	hLayout->addWidget(textEdit);

	mainLayout->addLayout(hLayout);

	// Color button
	colorButton = new QPushButton("Select Text Color", this);
	connect(colorButton, &QPushButton::clicked, this, &AddLocationDialog::selectColor);
	mainLayout->addWidget(colorButton);

	// Icon button
	iconButton = new QPushButton("Select Icon", this);
	connect(iconButton, &QPushButton::clicked, this, &AddLocationDialog::selectIcon);
	mainLayout->addWidget(iconButton);

	setLayout(mainLayout);
}

void AddLocationDialog::setDefaultImage() {
	imageLabel->setPixmap(g_mediaManager->getIcon("PlaceHolder").pixmap(64,64));
}

void AddLocationDialog::selectColor() {
	QColor color = QColorDialog::getColor(Qt::white, this, "Select Label Color");
	if (color.isValid()) {
		labelColor = color;
		colorButton->setStyleSheet("background-color: " + color.name());
	}
}

void AddLocationDialog::selectIcon() {
	QString fileName = QFileDialog::getOpenFileName(this, "Select Icon", "", "Images (*.png *.xpm *.jpg)");
	if (!fileName.isEmpty()) {
		QPixmap pixmap(fileName);
		imageLabel->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio)); // Adjust the size as needed
		iconButton->setText(fileName); // Update button text with file name
	}
}