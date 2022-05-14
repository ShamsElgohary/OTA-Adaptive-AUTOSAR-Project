#include "../include/applicationExecutionMgr.hpp"
using namespace std;
using namespace ara::exec;
using namespace boost::filesystem;

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
            path y(rootPath + "/executables" + x);
            p.push_back(y);
        }
    }

    for (auto &x : p)
    {
        string p2 = x.string() + "/etc/execution_manifest.json";
        executables_.push_back(Executable{ApplicationManifest(p2), vector<Application>()});
        for (auto &app : executables_.back().manifest_.startUpConfigurations)
        {
            executables_.back().startupConfigurations_.push_back(Application(app, executables_.back().manifest_.name, executables_.back().manifest_.executable_path));
        }
        for (auto &app : executables_.back().startupConfigurations_)
        {
            for (auto function_group : app.configuration_.function_group_states)
            {
                for (auto state : function_group.second)
                {
                    function_groups_[function_group.first]->startupConfigurations_[state].push_back(&app);
                }
            }
        }
    }
    return true;
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
    FunctionGroupState::CtorToken token = FunctionGroupState::Preconstruct(functionGroup_Name, functionGroup_NewState);
    FunctionGroupState functionGroup(move(token));
    return setState(functionGroup);
}

bool ApplicationExecutionMgr::setState(FunctionGroupState fgs)
{
    auto apps = function_groups_[fgs.fg_name]->startupConfigurations_[fgs.fg_newState];
    for (auto &app : apps)
    {
        if (app->current_state != ExecutionState::Krunning)
        {
            transitionChanges_.toStart_.push_back(app);
        }
    }
    apps = function_groups_[fgs.fg_name]->startupConfigurations_[function_groups_[fgs.fg_name]->currentState_];
    bool flag = true;
    for (auto &app : apps)
    {
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
                transitionChanges_.toTerminate_.push_back(app);
        }
    }
    function_groups_[fgs.fg_name]->currentState_ = fgs.fg_newState;
    return true;
}

void ApplicationExecutionMgr::initialize()
{
    mkfifo("smFifo", 0777);
    loadMachineConfigrations();
    loadExecutablesConfigrations();
    reportConfig_simulation();
    iam_future = IAM_handle();
    FunctionGroupState FGS(FunctionGroupState::Preconstruct("machineFG", "startup"));
    setState(FGS);
    Execute();
    transitionChanges_.toStart_.clear();
    transitionChanges_.toTerminate_.clear();
}

ApplicationExecutionMgr::ApplicationExecutionMgr(string rootPath) : rootPath{rootPath} {}

bool ApplicationExecutionMgr::run()
{
    string functionGroup_Name;
    string functionGroup_NewState;
    int size;
    while (true)
    {
        ProcessStateClientRequest();
        reportConfig_simulation();
        Terminate();
        Execute();
        transitionChanges_.toStart_.clear();
        transitionChanges_.toTerminate_.clear();
    }
}

bool ApplicationExecutionMgr::Terminate()
{
    for (auto &app : transitionChanges_.toTerminate_)
    {
        app->terminate();
        process_state_update_future.push_back(app->Update_status());
    }
}
bool ApplicationExecutionMgr::Execute()
{
    map<string, Application *> apps_state;
    queue<Application *> apps;
    for (auto app : transitionChanges_.toStart_)
    {
        apps_state[app->name] = app;
        apps.push(app);
    }
    bool flag = true;
    while (!apps.empty())
    {
        auto *app = apps.front();
        apps.pop();
        for (auto depend : app->configuration_.dependency)
        {
            if (apps_state[depend.first]->current_state != (depend.second == "Krunning" ? ExecutionState::Krunning : ExecutionState::Kterminate))
            {
                apps.push(app);
                flag = false;
                break;
            }
        }
        if (flag)
        {
            app->start();
            process_state_update_future.push_back(app->Update_status());
        }
        flag = true;
    }
    return true;
}
// future<void> ApplicationExecutionMgr::updateProcessState()
// {

//     return async(launch::async, [this]()
//                  {
//         while(1)
//         {
//             for (auto fng : function_groups_)
//             {
//                 for (auto state : function_groups_[fng.first]->startupConfigurations_)
//                 {
//                     for(auto app: state.second)
//                     {
//                         if(app->id !=NULL)
//                             app->Update_status();
//                     }
//                 }
//             }
//              std::this_thread::sleep_for(100ms);
//         } });
// }

string ApplicationExecutionMgr::get_process_name(int test_id)
{
    string p_name;
    for (auto &x : executables_)
    {
        for (auto &y : x.startupConfigurations_)
        {
            if (test_id == y.id)
            {
                p_name = y.name;
                return p_name;
            }
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
            cout<< "[em] " << process_name << endl;
            srv.sendData(process_name);
        } });
}

void ApplicationExecutionMgr::reportConfig_simulation()
{
    Json::Value machine_manifest_json;
    Json::Reader reader;
    std::ifstream input_file("../../etc/system/machine_manifest.json");
    reader.parse(input_file, machine_manifest_json);
    input_file.close();
    std::ofstream output_file("../etc/executables_config.json");

    Json::Value root;
    Json::Value vec(Json::arrayValue);
    Json::Value vec2(Json::arrayValue);
    Json::Value obj(Json::objectValue);
    Json::Value obj2(Json::objectValue);
    Json::Value obj3(Json::objectValue);
    root["function_groups"] = machine_manifest_json["function_groups"];
    for (auto app : executables_)
    {

        for (auto confg : app.manifest_.startUpConfigurations)
        {
            for (auto fg : confg.function_group_states)
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
        }
        obj["function_group"] = obj2;
        obj["name"] = app.manifest_.name;
        obj["depends"] = obj3;
        vec.append(obj);
        obj.clear();
        obj2.clear();
        obj3.clear();
        vec2.clear();
    }
    root["executables_configurations"] = vec;
    vec.clear();
    for (auto fng : function_groups_)
    {
        for (auto state : function_groups_[fng.first]->startupConfigurations_)
        {
            for (auto app : state.second)
            {
                if (app->id != 0)
                {
                    obj["name"] = app->name;
                    obj["current_state"] = (app->current_state == ExecutionState::Krunning ? "Krunning" : "Kterminate");
                    obj["pid"] = app->id;
                    vec.append(obj);
                    obj.clear();
                }
            }
        }
    }
    root["running_executables"] = vec;

    vec.clear();
    for (auto run : this->transitionChanges_.toStart_)
    {
        vec.append(run->name);
    }
    root["to_run"] = vec;
    for (auto term : this->transitionChanges_.toTerminate_)
    {
        vec.append(term->name);
    }
    vec.clear();
    root["to_term"] = vec;
    output_file << root;
    output_file.close();
}
