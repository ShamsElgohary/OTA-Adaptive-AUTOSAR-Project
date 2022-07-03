#include "exec.h"
#include "mainwindow.h"
#include "simulation.hpp"

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

    connect(this,SIGNAL(em_signal()),this,SLOT(update_exec()));

    update_executables_confg();
    update_machine_confg();
    update_running_executables();
    update_fng_states();
    update_sm_requests();

    setLayout(main_layout);
}
void exec::parse_exec_json()
{
    std::ifstream file_input(to_string(simulation::exe_name::exec));
    if(file_input.peek() == std::ifstream::traits_type::eof())return;
       Json::Reader reader;
       Json::Value root;
       reader.parse(file_input, root);
       bool flag =true;
       for(auto exe : root["executables_configurations"])
       {

           for(auto conf : exe["confg"])
           {
               exe_configuartions exe_conf;

               exe_conf.funtion_group =  conf["function_group"].getMemberNames()[0];
               for(auto state :  conf["function_group"][exe_conf.funtion_group])
               {
                   exe_conf.states.push_back(state.asString());
               }
               auto deps = conf["depends"].getMemberNames();
               for(auto dep : deps )
               {
                   exe_conf.dependancy[dep] = (conf["depends"][dep].asString());
               }
               if(flag)
               {
                   exes_conf[exe["name"].asCString()] =vector<exe_configuartions>();
                   flag =false ;
               }
               exes_conf[exe["name"].asCString()].push_back(exe_conf);
           }
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
       //----------------------------------------------
       auto fng_States = root["function_group_states"].getMemberNames();

       for(auto fn : fng_States)
       {
          fng_state[fn]=  root["function_group_states"][fn].asCString();
       }
       //------------------------------------------------
       sm_request["fng"] = root["sm_request"]["fng"].asCString();
       sm_request["fng_state"] = root["sm_request"]["fng_state"].asCString();

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
    for(auto exe :  exes_conf )
     {
        QTreeWidgetItem* name =  new QTreeWidgetItem{exec_tree};
        name->setText(0,QString::fromStdString(exe.first));
        font.setPointSize(12);
        font.setBold(true);
        name->setFont(0 ,font);

        name->setExpanded(true);
        int i=1;
        for(auto conf: exe.second)
        {
            QTreeWidgetItem* conf_node =  new QTreeWidgetItem{name};
            QString n = "conf"+ QString::number(i) ;
            conf_node->setText(0,n);
            font.setPointSize(12);
            font.setBold(false);
            conf_node->setFont(0 ,font);
            i++;

            QTreeWidgetItem* fng =  new QTreeWidgetItem{};
            font.setPointSize(12);
            font.setBold(false);

            fng->setText(0,"Function group");
            fng->setFont(0 ,font);
            conf_node->addChild(fng);

            QTreeWidgetItem* dep =  new QTreeWidgetItem{};
            dep->setText(0,"Dependency");
            dep->setFont(0 ,font);
            conf_node->addChild(dep);

            QTreeWidgetItem* fng_states =  new QTreeWidgetItem{};
            fng_states->setText(1,QString::fromStdString(conf.funtion_group));
            fng_states->setFont(1 ,font);
            fng->addChild(fng_states);

            for(auto &state : conf.states)
            {
                 QTreeWidgetItem* s =  new QTreeWidgetItem{};
                 s->setText(2,QString::fromStdString(state));
                 s->setFont(2 ,font);

                 fng_states->addChild(s);
            }
            for(auto &dependency : conf.dependancy)
            {
                 QTreeWidgetItem* d_name =  new QTreeWidgetItem{};
                 d_name->setText(1,(QString::fromStdString(dependency.first)));
                 d_name->setText(2,(QString::fromStdString(dependency.second)));
                 d_name->setFont(2 ,font);
                 d_name->setFont(1 ,font);
                 dep->addChild(d_name);
            }
            fng_states->setExpanded(true);
            fng->setExpanded(true);
            dep->setExpanded(true);
            conf_node->setExpanded(true);
        }
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
    int i=0;
    rexe_table->setRowCount(r_exe.size());
    for(auto exe : r_exe )
    {
        rexe_table->setItem(i,0, new QTableWidgetItem{QString::fromStdString(exe.name) });
        rexe_table->setItem(i,1, new QTableWidgetItem{QString::fromStdString(exe.current_state) });
        rexe_table->setItem(i,2, new QTableWidgetItem{QString::number(exe.pid) });
        i++;
    }

}
void exec::update_network_consle()
{
    std::ifstream file_input2;
    file_input2.open("/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/executables/em/bin/em_report.txt");
    string s ,s2;
    if(!file_input2) { // file couldn't be opened
          cout << "Error: file could not be opened" << endl;
       }
    else{
    while ( !file_input2.eof() ) {
         getline(file_input2,s);
         s2+= s+"\n";
      }
      file_input2.close();
    }
    network_text->setText(QString::fromStdString(s2));
}
void exec::update_fng_states()
{
    QFont font;
    font.setPointSize(12);
    font.setBold(true);

    fng_states->setColumnCount(1);
    fng_states->setHorizontalHeaderItem(0,new QTableWidgetItem {QString::fromStdString("current state")});

    fng_states->setRowCount(fng_state.size());
    int i=0;

    for(auto &fn: fng_state)
    {
        QTableWidgetItem *f = new QTableWidgetItem {QString::fromStdString(fn.first)};
        QTableWidgetItem *s = new QTableWidgetItem {QString::fromStdString(fn.second)};
        f->setFont(font);
        s->setFont(font);
        f->setTextAlignment(Qt::AlignCenter);
        s->setTextAlignment(Qt::AlignCenter);

        fng_states->setVerticalHeaderItem(i,f);
        fng_states->setItem(i-1,1,s);
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
    font.setPointSize(12);
    font.setBold(true);
    sm1_list->setColumnCount(2);
    if(sm_request.size()>0)
    {
        sm1_list->setRowCount(sm_request.size()-1);

    }
    else
    {
        sm1_list->setRowCount(0);

    }

    sm1_list->setHorizontalHeaderItem(0,new QTableWidgetItem {QString::fromStdString("function group")});
    sm1_list->setHorizontalHeaderItem(1,new QTableWidgetItem {QString::fromStdString("new state")});
    sm1_list->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    font.setPointSize(11);
    sm1_list->horizontalHeader()->setFont(font);
    sm1_list->setItem(0,0,new QTableWidgetItem {QString::fromStdString(sm_request["fng"])});
    sm1_list->setItem(0,1,new QTableWidgetItem {QString::fromStdString(sm_request["fng_state"])});

    sm2_list->setHeaderLabels(QStringList{"Action","Executable"});
    sm2_list->header()->setSectionResizeMode(QHeaderView::Stretch);
    sm2_list->header()->setFont(font);
    font.setPointSize(12);
    QTreeWidgetItem* tr =  new QTreeWidgetItem{sm2_list};
    QTreeWidgetItem* tt=  new QTreeWidgetItem{sm2_list};
    if(to_run.size()>0||to_term.size()>0)
    {
        tr->setText(0 ,"to_run");
        tt->setText(0 ,"to_terminate");
        tr->setFont(0 ,font);
        tt->setFont(0 ,font);
    }

    font.setBold(false);
    for(auto &exe : to_run)
    {
         QTreeWidgetItem* s =  new QTreeWidgetItem{};
         s->setText(1,QString::fromStdString(exe));
         s->setFont(1 ,font);
         tr->addChild(s);
    }
    for(auto &exe : to_term)
    {
         QTreeWidgetItem* s =  new QTreeWidgetItem{};
         s->setText(1,QString::fromStdString(exe));
         s->setFont(1 ,font);
         tt->addChild(s);
    }
    tt->setExpanded(true);
    tr->setExpanded(true);

}

void exec::clear_widget()
{
    exes_conf.clear();
    r_exe.clear();
    to_run.clear();
    to_term.clear();
    exec_tree->clear();
    machine_tree->clear();
    sm1_list->clear();
    sm2_list->clear();
    rexe_table->clear();
    fng_states->clear();
    sm_request.clear();
    network_text->clear();
}
void exec::update_exec( )
{
    clear_widget();
    parse_exec_json();
    update_executables_confg();
    update_machine_confg();
    update_running_executables();
    update_fng_states();
    update_sm_requests();
    update_network_consle();
}
void exec::em_connect()
{
    int c=1;
    string path(CUSTOMIZED_PROJECT_PATH+"gui_em");
    fd=open(path.c_str(), O_WRONLY);
    write(fd,&c,sizeof(int));
    ::close(fd);
}

void exec::em_handler()
{
    emit em_signal();
}
