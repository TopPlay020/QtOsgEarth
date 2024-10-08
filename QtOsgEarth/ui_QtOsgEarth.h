/********************************************************************************
** Form generated from reading UI file 'QtOsgEarthXFyPiH.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef QTOSGEARTHXFYPIH_H
#define QTOSGEARTHXFYPIH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include "OsgEarthManager.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction* actionOpen_File;
    QAction* actionEdit_File;
    QAction* actionreveal;
    OsgEarthManager* centralwidget;
    QStatusBar* statusbar;
    QMenuBar* menubar;
    QMenu* menuFile;
    QMenu* menuOpen_Recent_Files;

    void setupUi(QMainWindow* MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 598);
        actionOpen_File = new QAction(MainWindow);
        actionOpen_File->setObjectName("actionOpen_File");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/File"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_File->setIcon(icon);
        actionEdit_File = new QAction(MainWindow);
        actionEdit_File->setObjectName("actionEdit_File");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icons/Edit"), QSize(), QIcon::Normal, QIcon::Off);
        actionEdit_File->setIcon(icon1);
        actionreveal = new QAction(MainWindow);
        actionreveal->setObjectName("actionreveal");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Icons/Folder"), QSize(), QIcon::Normal, QIcon::Off);
        actionreveal->setIcon(icon2);
        centralwidget = new OsgEarthManager(MainWindow);
        centralwidget->setObjectName("centralwidget");
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuOpen_Recent_Files = new QMenu(menuFile);
        menuOpen_Recent_Files->setObjectName("menuOpen_Recent_Files");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Icons/RecentFile"), QSize(), QIcon::Normal, QIcon::Off);
        menuOpen_Recent_Files->setIcon(icon3);
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_File);
        menuFile->addAction(menuOpen_Recent_Files->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionEdit_File);
        menuFile->addAction(actionreveal);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow* MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen_File->setText(QCoreApplication::translate("MainWindow", "Open File", nullptr));
        actionEdit_File->setText(QCoreApplication::translate("MainWindow", "Edit File", nullptr));
        actionreveal->setText(QCoreApplication::translate("MainWindow", "Reveal in File explorer", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuOpen_Recent_Files->setTitle(QCoreApplication::translate("MainWindow", "Open Recent Files", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow : public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // QTOSGEARTHXFYPIH_H
