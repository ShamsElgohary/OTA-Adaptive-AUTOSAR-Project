#include "../include/applicationExecutionMgr.hpp"
using namespace std;
using namespace ara::exec;
using namespace boost::filesystem;
std::ofstream outdata;

bool ApplicationExecutionMgr::loadExecutablesConfigrations()
{
    Json::Value v;
    Json::Reader r;
    std::ifstream is;
    is.open("../../etc/system/Process_List.json");
    r.parse(is, v);
    vector<path> p;

    for (Json::Value::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        string key = it.key().asString();
        if (key != "Process List Version")
        {
            string x = v[key]["Path"].asString();
            path y(rootPath + "executables" + x);
            p.push_back(y);
        }
    }

    for (auto &x : p)
    {
        string p2 = x.string() + "etc/execution_manifest.json";
        executables_.push_back(Executable{ApplicationManifest(p2), vector<Application *>()});
        for (auto &app : executables_.back().manifest_.startUpConfigurations)
        {
            executables_.back().startupConfigurations_.push_back(new Application{app, executables_.back().manifest_.name, executables_.back().manifest_.executable_path});
        }
        for (auto &app : executables_.back().startupConfigurations_)
        {
            for (auto function_group : app->configuration_.function_group_states)
            {
                for (auto state : function_group.second)
                {
                    function_groups_[function_group.first]->startupConfigurations_[state].push_back(app);
                }
            }
        }
        excutables_map.insert({executables_.back().manifest_.name, executables_.back().manifest_.executable_path});
    }
}

bool ApplicationExecutionMgr::loadMachineConfigrations()
{
    manifest_ = make_unique<MachineManifest>("../../etc/system/machine_manifest.json");
    for (auto &fn : manifest_->function_groups)
    {
        for (auto &state : fn.allStates_)
            fn.startupConfigurations_.insert({state, vector<ara::exec::Application *>{}});
        function_groups_.insert({fn.name_, &fn});
    }
    return true;
}

bool ApplicationExecutionMgr::ProcessStateClientRequest()
{

    int size;
    char functionGroup_Name[10], functionGroup_NewState[10];
    smpipe = open("smFifo", O_RDONLY);
    read(smpipe, &size, sizeof(int));
    for (int i = 0; i <= size; i++)
    {
        read(smpipe, &functionGroup_Name[i], sizeof(char));
    }
    read(smpipe, &size, sizeof(int));
    for (int i = 0; i <= size; i++)
    {
        read(smpipe, &functionGroup_NewState[i], sizeof(char));
    }
    close(smpipe);
    outdata << "[SM] function group : " << functionGroup_Name << " new state is " << functionGroup_NewState << endl;
    outdata.flush();
    FunctionGroupState::CtorToken token = FunctionGroupState::Preconstruct(functionGroup_Name, functionGroup_NewState);
    FunctionGroupState functionGroup(move(token));
    newfunctionGroup = functionGroup;
    return setState(functionGroup);
}

bool ApplicationExecutionMgr::setState(FunctionGroupState fgs)
{
    auto &apps = function_groups_[fgs.fg_name]->startupConfigurations_[fgs.fg_newState];
    for (auto &app : apps)
    {
        unique_lock<mutex> locker(app->mur);
        if (app->current_state != ExecutionState::Krunning)
        {
            transitionChanges_.toStart_.push_back(app);
        }
        locker.unlock();
    }
    auto &apps_term = function_groups_[fgs.fg_name]->startupConfigurations_[function_groups_[fgs.fg_name]->currentState_];
    bool flag = true;
    for (auto &app : apps_term)
    {
        unique_lock<mutex> locker(app->mur);
        if (app->current_state == ExecutionState::Krunning)
        {
            for (auto state : app->configuration_.function_group_states[fgs.fg_name])
            {
                if (state == fgs.fg_newState)
                {
                    flag = false;
                }
            }
            if (flag)
            {
                transitionChanges_.toTerminate_.push_back(app);
            }
        }
        locker.unlock();
    }
    function_groups_[fgs.fg_name]->currentState_ = fgs.fg_newState;
    return true;
}

void ApplicationExecutionMgr::initialize()
{
    mkfifo("smFifo", 0777);
    #ifdef SIMULATION_ACTIVE
    {
        debugging_mode=true;
        wait_for_gui(); 
    }
    #endif
    loadMachineConfigrations();
    loadExecutablesConfigrations();
    #ifdef SIMULATION_ACTIVE
    {
        reportConfig_simulation();
        wait_for_gui(); 
    }
    #endif
    iam_future = IAM_handle();
    FunctionGroupState FGS(FunctionGroupState::Preconstruct("machineFG", "startup"));
    setState(FGS);
    #ifdef SIMULATION_ACTIVE
    {
        reportConfig_simulation();
        wait_for_gui(); 
    }
    #endif
    Execute();
    transitionChanges_.toStart_.clear();
    transitionChanges_.toTerminate_.clear();
}

ApplicationExecutionMgr::ApplicationExecutionMgr(string rootPath) : rootPath{rootPath}
{
    outdata.open("em_report.txt");
    remove("ActionsLog.txt");
    remove("sm.txt");
    remove("NetworkMessages.txt");
    system("rm *.json");    
    system("rm *.zip");
    #ifdef SIMULATION_ACTIVE
    {
        sim_socket.connect_to_socket();
        sim_socket.send_exe_name(simulation::exe_name::exec);
    }
     #endif

}

bool ApplicationExecutionMgr::run()
{
    while (true)
    {
        ProcessStateClientRequest();
         #ifdef SIMULATION_ACTIVE
        {
        reportConfig_simulation();
        wait_for_gui();
        }
        #endif
        Terminate();
        Execute();
        transitionChanges_.toStart_.clear();
        transitionChanges_.toTerminate_.clear();
        reparse();
        report_success_sm();
    }
}

void ApplicationExecutionMgr::report_success_sm()
{
    bool state_check = true;
    smpipe = open("smFifo", O_WRONLY);
    write(smpipe, &state_check, sizeof(bool));
    close(smpipe);
}
bool ApplicationExecutionMgr::Terminate()
{
    for (auto &app : transitionChanges_.toTerminate_)
    {
        app->terminate();
    }
}
bool ApplicationExecutionMgr::Execute()
{
    map<string, Application *> apps_state;
    for (auto &app : transitionChanges_.toStart_)
    {
        apps_state[app->name] = app;
    }
    for (auto &app : transitionChanges_.toStart_)
    {
        app->depend = apps_state;
        app->parent = this;
        app->start();
    }
    for (auto &app : transitionChanges_.toStart_)
    {
        unique_lock<mutex> locker(app->mur);
        while (app->current_state != ExecutionState::Krunning)
            app->condr.wait(locker);
        locker.unlock();
    }
    return true;
}

string ApplicationExecutionMgr::get_process_name(int test_id)
{
    string p_name;
    for (auto &x : executables_)
    {
        for (auto &y : x.startupConfigurations_)
        {
            unique_lock<mutex> locker(y->mur);

            if (test_id == y->id)
            {
                p_name = y->name;
                return p_name;
            }
            locker.unlock();
        }
    }
}

future<void> ApplicationExecutionMgr::IAM_handle()
{
    return async(launch::async, [this]()
                 {
        FindProcessServer srv;
        int id;
        string process_name;
        while(1){
            id = srv.receiveData();
            process_name = get_process_name(id);
            outdata<<"[IAM] process id : "<<id<<endl;
            outdata<< "[EM] process name : " << process_name << endl;
            outdata.flush();
            srv.sendData(process_name);
        } });
}

void ApplicationExecutionMgr::reportConfig_simulation()
{
    unique_lock<mutex> locker(mu);
    Json::Value machine_manifest_json;
    Json::Reader reader;
    std::ifstream input_file("../../etc/system/machine_manifest.json");
    reader.parse(input_file, machine_manifest_json);
    input_file.close();
    std::ofstream output_file("../etc/executables_config.json");

    Json::Value root;
    Json::Value vec(Json::arrayValue);
    Json::Value vec2(Json::arrayValue);
    Json::Value vec3(Json::arrayValue);

    Json::Value obj(Json::objectValue);
    Json::Value obj2(Json::objectValue);
    Json::Value obj3(Json::objectValue);
    Json::Value obj4(Json::objectValue);

    root["function_groups"] = machine_manifest_json["function_groups"];
    root["Cluster_name"] = "em_json";
    int i = 0;
    for (auto &app : executables_)
    {
        for (auto &confg : app.manifest_.startUpConfigurations)
        {
            for (auto &fg : confg.function_group_states)
            {
                for (auto state : fg.second)
                {
                    vec2.append(Json::Value(state));
                }
                obj2[fg.first] = vec2;
            }
            for (auto depends : confg.dependency)
            {
                obj3[depends.first] = depends.second;
            }
            obj4["function_group"] = obj2;
            obj4["depends"] = obj3;
            vec3.append(obj4);
            obj4.clear();
            obj2.clear();
            obj3.clear();
            vec2.clear();
        }
        obj["name"] = app.manifest_.name;
        obj["confg"] = vec3;
        vec.append(obj);
        obj.clear();
        vec3.clear();
    }
    root["executables_configurations"] = vec;
    vec.clear();
    for (auto &exe : executables_)
    {
        for (auto &app : exe.startupConfigurations_)
        {
            unique_lock<mutex> locker2(app->mur);
            if (app->current_state == ExecutionState::Krunning)
            {
                obj["name"] = app->name;
                obj["current_state"] = (app->current_state == ExecutionState::Krunning ? "Krunning" : "Kterminate");
                obj["pid"] = app->id;
                vec.append(obj);
                obj.clear();
            }
            locker2.unlock();
        }
    }
    obj["fng"] = this->newfunctionGroup.fg_name;
    obj["fng_state"] = this->newfunctionGroup.fg_newState;
    root["sm_request"] = obj;
    obj.clear();
    root["running_executables"] = vec;
    vec.clear();
    for (auto &run : this->transitionChanges_.toStart_)
    {
        vec.append(run->name);
    }
    root["to_run"] = vec;
    vec.clear();

    for (auto &term : this->transitionChanges_.toTerminate_)
    {
        vec.append(term->name);
    }
    root["to_term"] = vec;
    vec.clear();

    for (auto &fng : function_groups_)
    {
        obj[fng.first] = fng.second->currentState_;
    }
    root["function_group_states"] = obj;
    output_file << root;
    output_file.close();
    //------------------------------------------------
    sim_socket.send_file("../etc/executables_config.json");
    locker.unlock();
}
void ApplicationExecutionMgr::wait_for_gui()
{
    if(debugging_mode)
    {
        std::string path(CUSTOMIZED_PROJECT_PATH + "gui_em");
        printf("waiting\n");
        fd1 = open(path.c_str(), O_RDONLY);
        bool res;
        read(fd1, &res, sizeof(bool));
        this->debugging_mode=res;
        close(fd1);
    }
}

void ApplicationExecutionMgr::reparse()
{
    Json::Value v;
    Json::Reader r;
    std::ifstream is;
    is.open("../../etc/system/Process_List.json");
    r.parse(is, v);
    map<string, path> plist;
    int count = 0;

    std::map<string, string>::iterator it2;

    for (Json::Value::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        string key = it.key().asString();
        if (key != "Process List Version")
        {
            string x = v[key]["Path"].asString();
            path y(rootPath + "executables" + x);
            plist.insert({key, y});
        }
    }

    for (auto &k : plist)
    {
        it2 = excutables_map.find(k.first);
        if (it2 == excutables_map.end())
        {

            string p2 = k.second.string() + "etc/execution_manifest.json";

            executables_.push_back(Executable{ApplicationManifest(p2), vector<Application *>()});
            for (auto &app : executables_.back().manifest_.startUpConfigurations)
            {
                executables_.back().startupConfigurations_.push_back(new Application{app, executables_.back().manifest_.name, executables_.back().manifest_.executable_path});
            }
            for (auto &app : executables_.back().startupConfigurations_)
            {
                for (auto function_group : app->configuration_.function_group_states)
                {
                    for (auto state : function_group.second)
                    {
                        function_groups_[function_group.first]->startupConfigurations_[state].push_back(app);
                    }
                }
            }
        }
        else
        {
            string pname = k.second.string() + "bin/";
            if (pname != it2->second)
            {
                string n = k.first;
                for (auto i : executables_)
                {
                    if (i.manifest_.name == n)
                    {
                        i.manifest_.executable_path = pname;
                        for (auto &x : i.startupConfigurations_)
                        {
                            x->executable_path = pname;
                        }
                    }
                }
            }
        }
    }
}
