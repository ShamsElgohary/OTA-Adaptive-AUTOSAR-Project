#ifndef IAM_H
#define IAM_H

#include<QTableWidgetItem>
#include "fstream"
#include <QTableWidget>
#include "iostream"
#include "./jsoncpp/header/json.h"
#include "simulation.hpp"
#include <QWidget>
#include <QtWidgets>
#include <QStringList>

class IAM :public QWidget
{
     Q_OBJECT
public:
    explicit IAM(QWidget *parent = nullptr);
    //void PARSEIAM();
    void iam_handler();
public slots:
    void PARSEIAM();

signals:
    void iam_signal();
private:
    QStringList *columnsheader=new QStringList;
    QTextBrowser *console_text = new QTextBrowser;
    QGroupBox *console = new QGroupBox("Console", this);
    QTableWidget *request=new QTableWidget;
    QTableWidget *access=new QTableWidget;
    //QTableWidget *table=new QTableWidget;
    QGridLayout *main_layout = new QGridLayout;
    QGroupBox *AccesscontrollistGBX = new QGroupBox("Access Control List", this);
    QGroupBox *GantrequestsGBX = new QGroupBox("Grant Requests", this);
    simulation* sim=nullptr;
};


#endif
