#include "exec.h"

exec::exec(QWidget *parent): QWidget{parent}
{

    main_layout->addWidget(sm_requestsGbx, 0, 0, 1, 1 );
    main_layout->addWidget(machine_configGbx, 1 ,0, 2, 1);

    main_layout->addWidget(exec_configGbx, 0 ,1, 2, 1);
    main_layout->addWidget(Running_executables, 2 ,1, 1, 1);

    main_layout->addWidget(newtwork_console_Gbx, 3 ,0, 1, 2);


    machine_configGbx->setLayout(new QHBoxLayout);
    machine_configGbx->layout()->addWidget(machine_tree);

    exec_configGbx->setLayout(new QHBoxLayout);
    exec_configGbx->layout()->addWidget(exec_list);

    sm_requestsGbx->setLayout(new QHBoxLayout);
    sm_requestsGbx->layout()->addWidget(sm1_list);
    sm_requestsGbx->layout()->addWidget(sm2_list);

    newtwork_console_Gbx->setLayout(new QHBoxLayout);
    newtwork_console_Gbx->layout()->addWidget(network_text);



    Running_executables->setLayout(new QHBoxLayout);
    Running_executables->layout()->addWidget(rexe_list);



    setLayout(main_layout);
}
voidexec::update_exec()
{

}
