#include "exec.h"

exec::exec(QWidget *parent): QWidget{parent}
{
    machine_configGbx->setLayout(new QHBoxLayout());

    main_layout->addWidget(machine_configGbx, 0, 0, 2, 1);
    main_layout->addWidget(exec_configGbx, 0 ,1, 4, 1);
    main_layout->addWidget(sm_requestsGbx, 2 ,0, 2, 1);

    machine_configGbx->setLayout(new QHBoxLayout);
    machine_configGbx->layout()->addWidget(new QTreeWidget);

    exec_configGbx->setLayout(new QHBoxLayout);
    exec_configGbx->layout()->addWidget(new QListWidget);


    sm_requestsGbx->setLayout(new QHBoxLayout);
    sm_requestsGbx->layout()->addWidget(new QListWidget);
    sm_requestsGbx->layout()->addWidget(new QListWidget);


    setLayout(main_layout);
}
