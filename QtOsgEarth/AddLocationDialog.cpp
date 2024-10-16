#include <globals.h>
#include "AddLocationDialog.h"
#include <QApplication>
#include <QMainWindow>
#include <QAction>
#include <QMenuBar>

AddLocationDialog::AddLocationDialog(QWidget* parent) : QDialog(parent) {
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QHBoxLayout* TopLayout = new QHBoxLayout(this);

	// Image display label
	imageLabel = new QLabel(this);
	setDefaultImage(); // Set the default image
	imageLabel->setAlignment(Qt::AlignCenter); // Center the label
	imageLabel->setFixedSize(120, 120);
	TopLayout->addWidget(imageLabel, 0, Qt::AlignCenter); // Center the label in the main layout

	// Create a horizontal layout for name and text edits
	QVBoxLayout* vLayout = new QVBoxLayout();

	nameEdit = new QLineEdit(this);
	nameEdit->setPlaceholderText("Enter Label Name");
	nameEdit->setStyleSheet("QLineEdit {"
		"border: 2px solid #55b5ff;"  // Green border
		"border-radius: 5px;"
		"padding: 5px;"
		"font-size: 14px;"
		"}");
	vLayout->addWidget(nameEdit);

	textEdit = new QLineEdit(this);
	textEdit->setPlaceholderText("Enter Label Text");
	textEdit->setStyleSheet("QLineEdit {"
		"border: 2px solid #55b5ff;"  // Green border
		"border-radius: 5px;"
		"padding: 5px;"
		"font-size: 14px;"
		"}");
	vLayout->addWidget(textEdit);

	// Color button
	btn_color = new QPushButton("Select Text Color", this);
	btn_color->setCursor(Qt::PointingHandCursor);
	connect(btn_color, &QPushButton::clicked, this, &AddLocationDialog::selectColor);
	btn_color->setStyleSheet("QPushButton {"
		"background-color: #55b5ff;" // Green background
		"color: white;"
		"border: none;"
		"border-radius: 5px;"
		"padding: 8px;"
		"font-size: 14px;"
		"}"
		"QPushButton:hover {"
		"background-color: #4cae4f;"  // Darker green on hover
		"}");
	vLayout->addWidget(btn_color);

	// Icon button
	btn_icon = new QPushButton("Select Icon", this);
	btn_icon->setCursor(Qt::PointingHandCursor);
	connect(btn_icon, &QPushButton::clicked, this, &AddLocationDialog::selectIcon);
	btn_icon->setStyleSheet("QPushButton {"
		"background-color: #55b5ff;" // Green background
		"color: white;"
		"border: none;"
		"border-radius: 5px;"
		"padding: 8px;"
		"font-size: 14px;"
		"}"
		"QPushButton:hover {"
		"background-color: #4cae4f;"  // Darker green on hover
		"}");
	vLayout->addWidget(btn_icon);

	TopLayout->addLayout(vLayout);

	QHBoxLayout* BottomLayout = new QHBoxLayout();
	btn_addLabel = new QPushButton("Add Label" , this);
	btn_addLabel->setCursor(Qt::PointingHandCursor);
	connect(btn_addLabel, &QPushButton::clicked, this, &AddLocationDialog::accept);
	btn_addLabel->setStyleSheet("QPushButton {"
		"background-color: #55b5ff;" // Green background
		"color: white;"
		"border: none;"
		"border-radius: 5px;"
		"padding: 8px;"
		"font-size: 14px;"
		"}"
		"QPushButton:hover {"
		"background-color: #4cae4f;"  // Darker green on hover
		"}");

	BottomLayout->addWidget(btn_addLabel);

	mainLayout->addLayout(TopLayout);
	mainLayout->addLayout(BottomLayout);

	setLayout(mainLayout);
}

void AddLocationDialog::setDefaultImage() {
	imageLabel->setPixmap(g_mediaManager->getIcon("PlaceHolder").pixmap(120, 120));
}

void AddLocationDialog::selectColor() {
	QColor selectedColor = QColorDialog::getColor(Qt::green, this, "Select Color");
	if (selectedColor.isValid()) {
		btn_color->setStyleSheet(QString("QPushButton {"
			"background-color: %1;"
			"color: white;"
			"border: none;"
			"border-radius: 5px;"
			"padding: 8px;"
			"font-size: 14px;"
			"}"
			"QPushButton:hover {"
			"background-color: #4cae4f;"  // Darker green on hover
			"}").arg(selectedColor.name()));
	}
}

void AddLocationDialog::selectIcon() {
	QString fileName = QFileDialog::getOpenFileName(this, "Select Icon", "", "Images (*.png *.xpm *.jpg)");
	if (!fileName.isEmpty()) {
		QPixmap pixmap(fileName);
		imageLabel->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio)); // Adjust the size as needed
	}
}

void AddLocationDialog::execDialog() {
	setFixedSize(400, 200);
	textEdit->setFocus(); // Set focus on textEdit before exec
	exec(); // Execute the dialog
}