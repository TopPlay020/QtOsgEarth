#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QFileDialog>
#include <QLabel>
#include <QPixmap>

class AddLocationDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddLocationDialog(QWidget* parent = nullptr);

private slots:
    void selectColor();
    void selectIcon();

private:
    void setDefaultImage();

    QLineEdit* nameEdit;
    QLineEdit* textEdit;
    QPushButton* colorButton;
    QPushButton* iconButton;
    QLabel* imageLabel; // Label for displaying the image
    QColor labelColor;
};
