#include "ucm.h"

ucm::ucm(QWidget *parent, GUI_COMM *ptr)
    : QWidget{parent}
{
    com_ptr = ptr;
    main_layout->addWidget(ucmBox,0,0,1,4);


    //UCMmain GridBox setup
    ucmBox->setLayout(new QHBoxLayout);
    log->setLayout(new QHBoxLayout);
    mid->setLayout(new QVBoxLayout);
    clusters->setLayout(new QHBoxLayout);


    //UCMmain layout setup
    ucmBox->layout()->addWidget(log);
    ucmBox->layout()->addWidget(mid);
    ucmBox->layout()->addWidget(clusters);


    //MID Colume setup
    status->setLayout(new QHBoxLayout);
    newSwPkg->setLayout(new QVBoxLayout);
    activate->setLayout(new QVBoxLayout);
    transferRate->setLayout(new QVBoxLayout);

    //MID layout setup
    mid->layout()->addWidget(status);
    mid->layout()->addWidget(transferRate);
    mid->layout()->addWidget(newSwPkg);
    mid->layout()->addWidget(activate);


    mid->setMaximumWidth(450);
    log->setMaximumWidth(450);
    newSwPkg->setMaximumHeight(170);
    status->setMaximumHeight(80);
    activate->setMaximumHeight(170);
    transferRate->setMaximumHeight(80);

    //New SW PKG Details Implimentaion
    detailsTable->setRowCount(2);
    detailsTable->setColumnCount(1);
    detailsTableXLabels<<"Action Type"<<"Package Status";
    detailsTable->setVerticalHeaderLabels(detailsTableXLabels);
    detailsTable->horizontalHeader()->setVisible(0);
    detailsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    detailsTable->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    detailsTable->setSelectionMode(QAbstractItemView::NoSelection);
    detailsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    detailsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    newSwPkg->layout()->addWidget(detailsTable);



    //New SW PKG Details Implimentaion
    detailsTable2->setRowCount(2);
    detailsTable2->setColumnCount(1);
    detailsTableXLabels2<<"Cluster Name   "<<"Version";
    detailsTable2->setVerticalHeaderLabels(detailsTableXLabels2);
    detailsTable2->horizontalHeader()->setVisible(0);
    detailsTable2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    detailsTable2->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    detailsTable2->setSelectionMode(QAbstractItemView::NoSelection);
    detailsTable2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    detailsTable2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    newSwPkg->layout()->addWidget(detailsTable2);

    //Status Table Implimentaion

    statusTable->setRowCount(1);
    statusTable->setColumnCount(1);
    statusTableXLabels<<"Package Manger Status";
    statusTable->setVerticalHeaderLabels(statusTableXLabels);
    statusTable->horizontalHeader()->setVisible(0);
    statusTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    statusTable->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    statusTable->setSelectionMode(QAbstractItemView::NoSelection);
    statusTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    statusTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    status->layout()->addWidget(statusTable);



    //Activate Progress Bar
    activateProgressBar->setStyleSheet("value:20;");
    activate->layout()->addWidget(activateProgressBar);
    activateProgressBar->setValue(0);



    //transfer Progress Bar
    transferProgressBar->setStyleSheet("value:20;");
    transferRate->layout()->addWidget(transferProgressBar);
    transferProgressBar->setValue(0);



    //activate Table Implimentaion

    activateTable->setRowCount(4);
    activateTable->setColumnCount(1);
    activateTableXLabels<<"Start Update Session"<<"Prepare Update"<<"Verify Update"<<"Stop Update Session";
    activateTable->setVerticalHeaderLabels(activateTableXLabels);
    activateTable->horizontalHeader()->setVisible(0);
    activateTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    activateTable->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    activateTable->setSelectionMode(QAbstractItemView::NoSelection);
    activateTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    activateTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    activate->layout()->addWidget(activateTable);


    //Cluster dynamic table setup

    clustersTable->setRowCount(rowCluster);
    clustersTable->setColumnCount(colCluster);
    clusterTableYLabels<<"Name"<<"Version";
    clustersTable->setHorizontalHeaderLabels(clusterTableYLabels);
    //clustersTable->verticalHeader()->setVisible(0);
    clustersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    clustersTable->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    clustersTable->setSelectionMode(QAbstractItemView::NoSelection);
    clustersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //clustersTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    clusters->layout()->addWidget(clustersTable);



    //log side setup




    log->layout()->addWidget(logList);






    //add widget conf(y,x,col,row)

   // main_layout->addWidget(comBox,0,4,1,1);


//    updateActivateRate();
//    updateTransferRate();
//    updateLog();
//    prjMangerStatus();
//    allClusters();
//    readActivateNewCluster();

    setLayout(main_layout);

    connect(this,SIGNAL(update_ucm()),this,SLOT(updateLog()));
    connect(this,SIGNAL(update_ucm()),this,SLOT(allClusters()));
    connect(this,SIGNAL(update_ucm()),this,SLOT(prjMangerStatus()));
    connect(this,SIGNAL(update_ucm()),this,SLOT(readActivateNewCluster()));
    connect(this,SIGNAL(update_ucm()),this,SLOT(updateTransferRate()));
    connect(this,SIGNAL(update_ucm()),this,SLOT(updateActivateRate()));
    com_ptr->parseManifest(serviceInstanceManifestPath);

}


void ucm::updateLog()
{
        logList->clear();
        QFile file(activeLogPath);
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        logList->setText(in.readAll());
        com_ptr->parse(reportPath);
}


void ucm::addCluster(QString name,QString version)
{
    clustersTable->setItem(rowCluster,0,new QTableWidgetItem(QString(name)));
    clustersTable->setItem(rowCluster,1,new QTableWidgetItem(QString(version)));
    rowCluster++;
}


void ucm::allClusters()
{
   Json::Value event;
    ifstream inputFile(guiReportPath);
    Json::Reader R;
    R.parse(inputFile, event);
    rowCluster =0;
    for (Json::Value::const_iterator it=event["ucm_json"]["PresentClusters"].begin(); it!=event["ucm_json"]["PresentClusters"].end(); ++it)
           {
               clustersTable->setRowCount(rowCluster+1);
               QString name = QString::fromStdString((event["ucm_json"]["PresentClusters"][it.key().asString()]["Name"]).asString());
               QString version = QString::fromStdString((event["ucm_json"]["PresentClusters"][it.key().asString()]["Version"]).asString());
               clustersTable->setItem(rowCluster,0,new QTableWidgetItem(QString(name)));
               clustersTable->setItem(rowCluster,1,new QTableWidgetItem(QString(version)));
               rowCluster++;
           }

}

void ucm::prjMangerStatus()
{
    Json::Value event;
    ifstream inputFile(guiReportPath);
    Json::Reader R;
    R.parse(inputFile, event);

    QString name = QString::fromStdString((event["ucm_json"]["GUI"]["PackageManagerStatus"]).asString());
    statusTable->setItem(0,0,new QTableWidgetItem(QString(name)));
}

void ucm::readActivateNewCluster()
{
    Json::Value event;
    ifstream inputFile(guiReportPath);
    Json::Reader R;
    R.parse(inputFile, event);

    QString action = QString::fromStdString((event["ucm_json"]["PKGDetails"]["Action"]).asString());
    QString pkgStatus = QString::fromStdString((event["ucm_json"]["PKGDetails"]["PackageStatus"]).asString());
    QString ClusterName = QString::fromStdString((event["ucm_json"]["PKGDetails"]["Clusters"]["sw1"]["name"]).asString());
    QString Version = QString::fromStdString((event["ucm_json"]["PKGDetails"]["Clusters"]["sw1"]["version"]).asString());
    detailsTable->setItem(0,0,new QTableWidgetItem(QString(action)));
    detailsTable->setItem(0,1,new QTableWidgetItem(QString(pkgStatus)));
    detailsTable2->setItem(0,0,new QTableWidgetItem(QString(ClusterName)));
    detailsTable2->setItem(0,1,new QTableWidgetItem(QString(Version)));
}


void ucm::updateTransferRate()
{
    Json::Value event;
    ifstream inputFile(guiReportPath);
    Json::Reader R;
    R.parse(inputFile, event);

    quint8 perc = (event["ucm_json"]["GUI"]["TransferFinishRate"]).asInt();
    transferProgressBar->setValue(perc);
}


void ucm::updateActivateRate()
{
    Json::Value event;
    ifstream inputFile(guiReportPath);
    Json::Reader R;
    R.parse(inputFile, event);

    QString prepareUpdate = QString::fromStdString((event["ucm_json"]["GUI"]["Activate"]["PrepareUpdate"]).asString());
    QString startUpdateSession = QString::fromStdString((event["ucm_json"]["GUI"]["Activate"]["StartUpdateSession"]).asString());
    QString verifyUpdate = QString::fromStdString((event["ucm_json"]["GUI"]["Activate"]["VerifyUpdate"]).asString());
    QString finish = QString::fromStdString((event["ucm_json"]["GUI"]["Activate"]["StopUpdateSession"]).asString());

    activateTable->setItem(0,1,new QTableWidgetItem(QString(prepareUpdate)));
    activateTable->setItem(0,0,new QTableWidgetItem(QString(startUpdateSession)));
    activateTable->setItem(0,2,new QTableWidgetItem(QString(verifyUpdate)));
    activateTable->setItem(0,3,new QTableWidgetItem(QString(finish)));

    if(startUpdateSession == "Done")
    {
        activateProgressBar->setValue(25);
    }
    if(prepareUpdate == "Prepared")
    {
        activateProgressBar->setValue(50);
    }
    if(verifyUpdate == "Verified")
    {
        activateProgressBar->setValue(75);
    }
    if(finish == "Stopped")
    {
        activateProgressBar->setValue(100);
    }

}


void ucm::ucm_handler()
{
    emit update_ucm();

}
