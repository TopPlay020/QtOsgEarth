#include <globals.h>
#include "AddLocationDialog.h"
#include <QApplication>
#include <QMainWindow>
#include <QAction>
#include <QMenuBar>

AddLocationDialog::AddLocationDialog(QWidget* parent) : QDialog(parent) {
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QHBoxLayout* TopLayout = new QHBoxLayout(this);

	QVBoxLayout* iconVLayout = new QVBoxLayout();
	TopLayout->addLayout(iconVLayout);
	// Image display label
	imageLabel = new QLabel(this);
	imageLabel->setAlignment(Qt::AlignCenter); // Center the label
	imageLabel->setFixedSize(120, 120);
	iconVLayout->addWidget(imageLabel); // Center the label in the main layout

	QVBoxLayout* sliderLayout = new QVBoxLayout();
	sliderLayout->setContentsMargins(0, 10, 0, 0);
	slider = new QSlider(Qt::Horizontal, this);
	slider->setRange(32, 64);
	slider->setValue(48);  // Default value
	slider->setStyleSheet(
		"QSlider::groove:horizontal {"
		"height: 8px;"
		"background: #ddd;"
		"border-radius: 4px;"
		"}"
		"QSlider::handle:horizontal {"
		"background-color: #55b5ff;"
		"border: none;"
		"width: 18px;"
		"margin: -5px 0;"  // Align the handle with the groove
		"border-radius: 9px;"
		"}"
		"QSlider::handle:horizontal:hover {"
		"background-color: #4cae4f;"  // Hover color
		"}"
	);

	// Add the slider below imageLabel in TopLayout
	sliderLayout->addWidget(slider, 0, Qt::AlignCenter);

	sliderValueLabel = new QLabel(QString::number(slider->value()), this);
	sliderValueLabel->setAlignment(Qt::AlignCenter);
	sliderValueLabel->setStyleSheet("font-size: 14px; padding: 2px;");

	connect(slider, &QSlider::valueChanged, this, [this](int value) {setIconSize(value);});

	sliderLayout->addWidget(sliderValueLabel, 0, Qt::AlignCenter);

	iconVLayout->addLayout(sliderLayout, Qt::AlignBottom);

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
	btn_addLabel = new QPushButton("Add Label", this);
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
	setDefaultImage();
}

void AddLocationDialog::setDefaultImage() {
	auto size = slider->value();
	imageLabel->setPixmap(g_mediaManager->getIcon("PlaceHolder").pixmap(size, size));
	iconUrl = g_mediaManager->getIconPath(g_mediaManager->getIcon("PlaceHolder")).c_str();
}

void AddLocationDialog::setIconSize(int size) {
	sliderValueLabel->setText(QString::number(size));
	QPixmap pixmap(iconUrl);
	imageLabel->setPixmap(pixmap.scaled(size, size, Qt::KeepAspectRatio));
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
		iconUrl = fileName;
		QPixmap pixmap(fileName);
		imageLabel->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio)); // Adjust the size as needed
	}
}

int AddLocationDialog::execDialog() {
	setFixedSize(400, 200);
	textEdit->setFocus(); // Set focus on textEdit before exec
	return exec(); // Execute the dialog
}

QString AddLocationDialog::getName() {
	return nameEdit->text();
}

QString AddLocationDialog::getText() {
	return textEdit->text();
}

QString AddLocationDialog::getIconUrl() {
	return iconUrl;
}

float AddLocationDialog::getIconSize() {
	return (float)slider->value() / (float)slider->maximum();
}

QColor AddLocationDialog::getTextColor() {
	return btn_color->palette().button().color();
}
