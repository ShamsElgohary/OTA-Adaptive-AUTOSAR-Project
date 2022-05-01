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
    bool success=setState(functionGroup);
    write(smpipe,&success,sizeof(success));
    return success;
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
    function_groups_[fgs.fg_name]->currentState_  =   fgs.fg_newState ;
    return true;
}

void ApplicationExecutionMgr::initialize()
{
    mkfifo("executablesFifo", 0777);
    mkfifo("smFifo", 0777);
    loadMachineConfigrations();
    loadExecutablesConfigrations();
    IAM_handle();
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
    }
}
bool ApplicationExecutionMgr::Execute()
{
    for (auto app : transitionChanges_.toStart_)
    {
        app->start();
        app->Update_status();
        if (app->current_state != ExecutionState::Krunning)
            return false;
    }
}

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

void ApplicationExecutionMgr::IAM_handle()
{
    iam_future = async(launch::async, [this]()
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
