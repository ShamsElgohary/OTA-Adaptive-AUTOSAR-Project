#include "ucm.h"

ucm::ucm(QWidget *parent)
    : QWidget{parent}
{
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
    newSwPkg->setLayout(new QHBoxLayout);
    activate->setLayout(new QHBoxLayout);

    //MID layout setup
    mid->layout()->addWidget(status);
    mid->layout()->addWidget(newSwPkg);
    mid->layout()->addWidget(activate);

    mid->setMaximumWidth(500);
    status->setMaximumHeight(160);

    //New SW PKG Details Implimentaion

    detailsTable->setRowCount(4);
    detailsTable->setColumnCount(1);
    detailsTableXLabels<<"Cluster Name"<<"Version"<<"Action"<<"Status";
    detailsTable->setVerticalHeaderLabels(detailsTableXLabels);
    detailsTable->horizontalHeader()->setVisible(0);
    detailsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    detailsTable->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    detailsTable->setSelectionMode(QAbstractItemView::NoSelection);
    detailsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    detailsTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    detailsTable->setItem(0,0,new QTableWidgetItem(QString("Hello")));


    status->layout()->addWidget(detailsTable);
    //add widget conf(y,x,col,row)

    main_layout->addWidget(comBox,0,4,1,1);



    setLayout(main_layout);
}
