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
#include <QSlider>


class AddLocationDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddLocationDialog(QWidget* parent = nullptr);
    int execDialog();

    QString getName();
    QString getText();
    QString getIconUrl();
    QColor getTextColor();

private slots:
    void selectColor();
    void selectIcon();
    void setIconSize(int size);

private:
    void setDefaultImage();

    QLineEdit* nameEdit;
    QLineEdit* textEdit;
    QPushButton* btn_color;
    QPushButton* btn_icon;
    QPushButton* btn_addLabel;
    QSlider* slider;
    QLabel* sliderValueLabel;
    QLabel* imageLabel; // Label for displaying the image
    QColor labelColor;

    QString iconUrl;
};
