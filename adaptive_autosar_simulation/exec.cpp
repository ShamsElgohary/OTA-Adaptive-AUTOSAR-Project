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
void exec::parse_exec_json()
{

    ifstream file_input("/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/executables/em/etc/executables_config.json");
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);

    exe_configuartions exe_conf;
    for(auto exe : root["executables_configurations"])
    {
        exe_conf.name =  exe["name"].asString();
        exe_conf.funtion_group =  exe["function_group"].getMemberNames()[0];
        for(auto state :  exe["function_group"][exe_conf.funtion_group])
        {
            exe_conf.states.push_back(state.asString());
        }
        auto deps = exe["function_group"]["depends"].getMemberNames();
        for(auto dep : deps )
        {
            exe_conf.dependancy[dep] = (exe["function_group"]["depends"][dep].asString());
        }
        exes_conf.push_back(exe_conf);
        exe_conf.states.clear();
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


void exec::update_exec()
{

}

