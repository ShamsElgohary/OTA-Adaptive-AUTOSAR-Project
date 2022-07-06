/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *EngineLayout;
    QPushButton *fowardBtn;
    QPushButton *leftBtn;
    QPushButton *rightBtn;
    QPushButton *backardBtn;
    QPushButton *stopEngine;
    QPushButton *startEngine;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(882, 616);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        EngineLayout = new QFrame(centralwidget);
        EngineLayout->setObjectName(QString::fromUtf8("EngineLayout"));
        EngineLayout->setEnabled(true);
        EngineLayout->setGeometry(QRect(120, 30, 600, 500));
        EngineLayout->setAutoFillBackground(false);
        EngineLayout->setFrameShape(QFrame::StyledPanel);
        EngineLayout->setFrameShadow(QFrame::Raised);
        fowardBtn = new QPushButton(EngineLayout);
        fowardBtn->setObjectName(QString::fromUtf8("fowardBtn"));
        fowardBtn->setGeometry(QRect(210, 30, 151, 81));
        QFont font;
        font.setPointSize(25);
        fowardBtn->setFont(font);
        leftBtn = new QPushButton(EngineLayout);
        leftBtn->setObjectName(QString::fromUtf8("leftBtn"));
        leftBtn->setGeometry(QRect(20, 150, 161, 91));
        leftBtn->setFont(font);
        rightBtn = new QPushButton(EngineLayout);
        rightBtn->setObjectName(QString::fromUtf8("rightBtn"));
        rightBtn->setGeometry(QRect(400, 150, 161, 91));
        rightBtn->setFont(font);
        backardBtn = new QPushButton(EngineLayout);
        backardBtn->setObjectName(QString::fromUtf8("backardBtn"));
        backardBtn->setGeometry(QRect(210, 280, 141, 81));
        backardBtn->setFont(font);
        stopEngine = new QPushButton(EngineLayout);
        stopEngine->setObjectName(QString::fromUtf8("stopEngine"));
        stopEngine->setGeometry(QRect(240, 390, 81, 31));
        stopEngine->setLayoutDirection(Qt::LeftToRight);
        startEngine = new QPushButton(EngineLayout);
        startEngine->setObjectName(QString::fromUtf8("startEngine"));
        startEngine->setGeometry(QRect(220, 150, 131, 71));
        QFont font1;
        font1.setPointSize(15);
        startEngine->setFont(font1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 882, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        fowardBtn->setText(QCoreApplication::translate("MainWindow", "\342\206\221", nullptr));
        leftBtn->setText(QCoreApplication::translate("MainWindow", "\342\206\220", nullptr));
        rightBtn->setText(QCoreApplication::translate("MainWindow", "\342\206\222", nullptr));
        backardBtn->setText(QCoreApplication::translate("MainWindow", "\342\206\223", nullptr));
        stopEngine->setText(QCoreApplication::translate("MainWindow", "Stop Engine", nullptr));
        startEngine->setText(QCoreApplication::translate("MainWindow", "Start Engine", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
