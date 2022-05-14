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



    //add widget conf(y,x,col,row)

    main_layout->addWidget(comBox,0,4,1,1);



    setLayout(main_layout);
}
