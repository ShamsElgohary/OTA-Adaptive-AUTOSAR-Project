#ifndef UCM_H
#define UCM_H

#include <QWidget>
#include <QtWidgets>
#include "jsoncpp/header/json.h"
#include <fstream>
#include <iostream>
#include "simulation.hpp"
#include"com.h"
#include "../utility/general.hpp"
using namespace std;
using namespace Json;

class ucm: public QWidget
{
    Q_OBJECT
public:
    explicit ucm(QWidget *parent = nullptr,GUI_COMM*ptr=nullptr);

public slots:
    void updateLog();
    void allClusters();
    void addCluster(QString name,QString version);
    void prjMangerStatus();
    void readActivateNewCluster();
    void updateTransferRate();
    void updateActivateRate();
    void ucm_handler();

signals:
    void update_ucm();

private:
     QGridLayout *main_layout = new QGridLayout;

     //UCMmain GridBox setup
     QGroupBox *ucmBox = new QGroupBox("UCM");
     QGroupBox *log = new QGroupBox("LOG");
     QGroupBox *mid = new QGroupBox();
     QGroupBox *clusters = new QGroupBox("Present Clusters");


     //MID Colume setup
     QGroupBox *status = new QGroupBox();
     QGroupBox *newSwPkg = new QGroupBox("Latest Processed Software Package Details");
     QGroupBox *activate = new QGroupBox("Activate Details");
     QGroupBox *transferRate = new QGroupBox("Transfer Progress");


     //New SW PKG Details
     QTableWidget *detailsTable= new QTableWidget;
     QTableWidget *detailsTable2= new QTableWidget;
     QTableWidget *statusTable= new QTableWidget;
     QLabel *actionText = new QLabel;
     QStringList detailsTableXLabels;
     QStringList detailsTableXLabels2;
     QStringList statusTableXLabels;


     //Activate progressbar
     QProgressBar *activateProgressBar = new QProgressBar;
     QTableWidget *activateTable= new QTableWidget;
     QStringList activateTableXLabels;


     //Activate progressbar
     QProgressBar *transferProgressBar = new QProgressBar;


     //Cluster dynamic table conf
     QTableWidget *clustersTable= new QTableWidget;
     quint8 rowCluster = 0;
     quint8 colCluster = 2;
     QStringList clusterTableYLabels;
     //Log side conf

     QTextBrowser *logList= new QTextBrowser;

     //Add lists to mid colume

    string guiReportPath = to_string(simulation::exe_name::ucm);
    QString activeLogPath = QString::fromStdString((CUSTOMIZED_PROJECT_PATH) + "executables/em/bin/ActionsLog.txt");
    //COM GridBox setup
     //QGroupBox *comBox = new QGroupBox("COM");

    GUI_COMM * com_ptr;
    QString reportPath = QString::fromStdString(CUSTOMIZED_PROJECT_PATH) +"executables/em/bin/ucm_Reporting.json";
    QString serviceInstanceManifestPath = QString::fromStdString(CUSTOMIZED_PROJECT_PATH) +"executables/ucm/0.1/etc/service_manifest.json";
};

#endif // UCM_H
