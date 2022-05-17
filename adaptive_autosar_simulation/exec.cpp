#include "exec.h"

exec::exec(QWidget *parent): QWidget{parent}
{
    machine_configGbx->setLayout(new QHBoxLayout());

    main_layout->addWidget(Running_executables, 0, 0, 1, 1 );
    main_layout->addWidget(sm_requestsGbx, 1 ,0, 1, 1);
    main_layout->addWidget(fng_states_gbx, 2 ,0, 1, 1);

    main_layout->addWidget(exec_configGbx, 0 ,1, 2, 1);
    main_layout->addWidget(machine_configGbx, 2 ,1, 2, 1);

    main_layout->addWidget(newtwork_console_Gbx, 3,0, 1, 1);


    machine_configGbx->setLayout(new QHBoxLayout);
    machine_configGbx->layout()->addWidget(machine_tree);

    exec_configGbx->setLayout(new QHBoxLayout);
    exec_configGbx->layout()->addWidget(exec_tree);
    exec_tree->resize(exec_configGbx->width() ,exec_configGbx->height() );
    exec_tree->setMinimumHeight(450);

    fng_states_gbx->setLayout(new QHBoxLayout);
    fng_states_gbx->layout()->addWidget(fng_states);

    QVBoxLayout * left=new QVBoxLayout;
    QVBoxLayout * right=new QVBoxLayout;
    QVBoxLayout * v=new QVBoxLayout;

    QHBoxLayout * htop=new QHBoxLayout;
    QHBoxLayout * hdown=new QHBoxLayout;

    sm_requestsGbx->setLayout(hdown);


    hdown->addLayout(left);
    hdown->addLayout(right);
    left->addWidget(sm1_list);
    right->addWidget(sm2_list);

    sm1_list->setMaximumHeight(250);
    sm2_list->setMaximumHeight(250);

    newtwork_console_Gbx->setLayout(new QHBoxLayout);
    newtwork_console_Gbx->layout()->addWidget(network_text);


    Running_executables->setLayout(new QHBoxLayout);
    Running_executables->layout()->addWidget(rexe_table);
    rexe_table->setMinimumHeight(220);

    parse_exec_json();
    update_exec();
    setLayout(main_layout);
}
void exec::parse_exec_json()
{

    ifstream file_input("/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/executables/em/etc/executables_config.json");
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);

    for(auto exe : root["executables_configurations"])
    {
        exe_configuartions exe_conf;
        exe_conf.name =  exe["name"].asString();
        exe_conf.funtion_group =  exe["function_group"].getMemberNames()[0];
        for(auto state :  exe["function_group"][exe_conf.funtion_group])
        {
            exe_conf.states.push_back(state.asString());
        }
        auto deps = exe["depends"].getMemberNames();
        for(auto dep : deps )
        {
            exe_conf.dependancy[dep] = (exe["depends"][dep].asString());
        }
        exes_conf.push_back(exe_conf);
    }
    //--------------------------------------------------------
    auto fns = root["function_groups"].getMemberNames();
    vector<string> states;
    for(auto fn :fns)
    {
        for(auto state : root["function_groups"][fn]["states"])
        {
            states.push_back(state.asString());
        }
        fng[fn] = states;
        states.clear();
    }
    //--------------------------------------------------------
    running_exec executable ;
    for(auto exe : root["running_executables"])
    {
        executable.name = exe["name"].asString();
        executable.current_state = exe["current_state"].asString();
        executable.pid = exe["pid"].asInt();
        r_exe.push_back(executable);
    }
    //--------------------------------------------------------
    for(auto exe : root["to_run"])
    {
       to_run.push_back(exe.asString());
    }
    //--------------------------------------------------------
    for(auto exe : root["to_term"])
    {
       to_term.push_back(exe.asString());
    }
}


void exec::update_machine_confg()
{
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    machine_tree->setColumnCount(2);
    machine_tree->setHeaderLabels(QStringList{"Function group","states"});
    machine_tree->header()->setSectionResizeMode(QHeaderView::Stretch);
    machine_tree->header()->setFont(font);

    for(auto fn : fng)
     {
        QTreeWidgetItem* name =  new QTreeWidgetItem{machine_tree};
        font.setPointSize(13);
        font.setBold(true);
        name->setText(0,QString::fromStdString(fn.first));
        name->setFont(0,font);

        for(auto &state : fn.second)
        {
            font.setPointSize(13);
            font.setBold(false);
             QTreeWidgetItem* s =  new QTreeWidgetItem{};
             s->setText(1,QString::fromStdString(state));
             s->setFont(1,font);

             name->addChild(s);
        }
        name->setExpanded(true);

    }

}
void exec::update_executables_confg()
{
    exec_tree->setColumnCount(3);
    exec_tree->setHeaderLabels(QStringList{"Executable","Configuration","states"});
    exec_tree->header()->setSectionResizeMode(QHeaderView::Stretch);
    QFont font;
    font.setPointSize(11);
    font.setBold(true);
    exec_tree->header()->setFont(font);
    for(int i = 0 ;i<exes_conf.size() ;i++)
     {
        QTreeWidgetItem* name =  new QTreeWidgetItem{exec_tree};
        name->setText(0,QString::fromStdString(exes_conf[i].name));
        font.setPointSize(12);
        font.setBold(true);
        name->setFont(0 ,font);
        QTreeWidgetItem* fng =  new QTreeWidgetItem{};
        font.setPointSize(12);
        font.setBold(false);

        fng->setText(0,"Function group");
        fng->setFont(0 ,font);
        name->addChild(fng);

        QTreeWidgetItem* dep =  new QTreeWidgetItem{};
        dep->setText(0,"Dependency");
        dep->setFont(0 ,font);
        name->addChild(dep);

        QTreeWidgetItem* fng_states =  new QTreeWidgetItem{};
        fng_states->setText(1,QString::fromStdString(exes_conf[i].funtion_group));
        fng_states->setFont(1 ,font);
        fng->addChild(fng_states);

        for(auto &state : exes_conf[i].states)
        {
             QTreeWidgetItem* s =  new QTreeWidgetItem{};
             s->setText(2,QString::fromStdString(state));
             s->setFont(2 ,font);

             fng_states->addChild(s);
        }
        for(auto &dependency : exes_conf[i].dependancy)
        {
             QTreeWidgetItem* d_name =  new QTreeWidgetItem{};
             d_name->setText(1,(QString::fromStdString(dependency.first)));
             d_name->setText(2,(QString::fromStdString(dependency.second)));
             dep->addChild(d_name);
        }
        name->setExpanded(true);

    }
}
void exec::update_running_executables()
{
    QFont font;
    font.setPointSize(11);
    font.setBold(true);

    rexe_table->setColumnCount(3);
    rexe_table->setRowCount(0);

    rexe_table->setHorizontalHeaderLabels(QStringList{"Name","Current state","PID"});
    rexe_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    rexe_table->horizontalHeader()->setFont(font);

    for(auto exe : r_exe )
    {
        rexe_table->setRowCount(rexe_table->rowCount()+1);
        rexe_table->setItem(rexe_table->rowCount()-1,0, new QTableWidgetItem{QString::fromStdString(exe.name) });
        rexe_table->setItem(rexe_table->rowCount()-1,1, new QTableWidgetItem{QString::fromStdString(exe.current_state) });
        rexe_table->setItem(rexe_table->rowCount()-1,2, new QTableWidgetItem{QString::number(exe.pid) });
    }

}
void exec::update_network_consle()
{

}
void exec::update_fng_states()
{
    QFont font;
    font.setPointSize(11);
    font.setBold(true);

    fng_states->setColumnCount(1);
    fng_states->setHorizontalHeaderItem(0,new QTableWidgetItem {QString::fromStdString("current state")});

    fng_states->setRowCount(fng.size());
    int i=0;
    for(auto &fn: fng)
    {
        fng_states->setVerticalHeaderItem(i,new QTableWidgetItem {QString::fromStdString(fn.first)});
        i++;
    }
    fng_states->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    fng_states->horizontalHeader()->setFont(font);
    fng_states->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    fng_states->verticalHeader()->setFont(font);

}
void exec::update_sm_requests()
{
    QFont font;
    font.setPointSize(11);
    font.setBold(true);
    sm1_list->setColumnCount(2);
    sm1_list->setHorizontalHeaderItem(0,new QTableWidgetItem {QString::fromStdString("function group")});
    sm1_list->setHorizontalHeaderItem(1,new QTableWidgetItem {QString::fromStdString("new state")});
    sm1_list->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    sm1_list->horizontalHeader()->setFont(font);

    sm2_list->setHeaderLabels(QStringList{"Action","Executable"});
    sm2_list->header()->setSectionResizeMode(QHeaderView::Stretch);
    sm2_list->header()->setFont(font);

}
void exec::update_exec()
{
    update_executables_confg();
    update_machine_confg();
    update_running_executables();
    update_fng_states();
    update_sm_requests();
}
