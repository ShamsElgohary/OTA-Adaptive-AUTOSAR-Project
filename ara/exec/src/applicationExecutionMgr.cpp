#include "../include/applicationExecutionMgr.hpp"
using namespace std;
using namespace ara::exec;
using namespace boost::filesystem;

//1-parses directories to reach execution manifests
//2-makes executable objects and passes its manifest path to its constructor
//3-push executable objects to executables_ vector
bool ApplicationExecutionMgr::loadExecutablesConfigrations()
{
    path p("../../../executables");
    try
    {
        if (exists(p))
        {
            if (is_directory(p))
            {
                cout << p << " is a directory containing:\n";

                for (directory_entry &x : directory_iterator(p))
                {
                    string p2 = x.path().string() + "/etc/" + x.path().filename().string() + ".json";
                    executables_.push_back(Executable{ApplicationManifest(p2), vector<Application>()});
                }
            }
            else
            {
                cout << p << " exists, but is not a regular file or directory\n";
            }
        }
        else
        {
            cout << p << " does not exist\n";
        }
    }
    catch (const filesystem_error &ex)
    {
        cout << ex.what() << '\n';
        return false;
    }
    return true;
}

//1-makes MachineManifest Object in heap and passses machine manifest path to its constructor
//2-sets the unique ptr to this object
bool ApplicationExecutionMgr::loadMachineConfigrations()
{
    manifest_ = make_unique<MachineManifest>("../../etc/system/machine_manifest.json");
    for(auto &fn : manifest_->function_groups)
    {
        for(auto &state :fn.allStates_)
            fn.startupConfigurations_.insert({state ,vector<ara::exec::Application *>{}}) ;
        function_groups_.insert({fn.name_ , fn});
    }
    return true;
}


bool ApplicationExecutionMgr::ProcessStateClientRequest()
{
    int size ;
    char functionGroup_Name [10] , functionGroup_NewState[10];
    cout<<"trying to open pipe from em \n";
    cout<<"pipe opened \n";
    if(smpipe==-1)
        smpipe = open("smFifo",O_RDONLY);
    read(smpipe, &size, sizeof(int));
    for(int i =0 ;i<=size ;i++)
    {
        read(smpipe, &functionGroup_Name[i], sizeof(char));
    }
    read(smpipe, &size, sizeof(int));
     for(int i =0 ;i<=size ;i++)
    {
        read(smpipe, &functionGroup_NewState[i], sizeof(char));
    }
    FunctionGroupState::CtorToken token = FunctionGroupState::Preconstruct(functionGroup_Name, functionGroup_NewState);
    FunctionGroupState functionGroup(move(token));
    return setState(functionGroup);
}

bool ApplicationExecutionMgr::setState(FunctionGroupState fgs)
{
    for (auto &ex : executables_)
    {
        for (auto &confg : ex.manifest_.startUpConfigurations)
        {
            if(fgs.fg_name !="machineState")
                for (auto &state : confg.function_group_states[fgs.fg_name])
                {
                    if (state == fgs.fg_newState)
                    {
                        ex.startupConfigurations_.push_back(Application(confg, ex.manifest_.name, ex.manifest_.executable_path));
                        function_groups_[fgs.fg_name].startupConfigurations_[state].push_back(&ex.startupConfigurations_.back());    
                        transitionChanges_.toStart_.push_back(&ex.startupConfigurations_.back());
                        break;
                    }
                }
            else{
                 for (auto &state : confg.machine_states)
                {
                    if (state == fgs.fg_newState)
                    {
                        ex.startupConfigurations_.push_back(Application(confg, ex.manifest_.name, ex.manifest_.executable_path));
                        function_groups_[fgs.fg_name].startupConfigurations_[state].push_back(&ex.startupConfigurations_.back());    
                        transitionChanges_.toStart_.push_back(&ex.startupConfigurations_.back());
                        break;
                    }
                }
            }
        }
    }
    auto fn =function_groups_[fgs.fg_name];
    if(fn.currentState_ != fgs.fg_newState){
        auto apps = fn.startupConfigurations_[fn.currentState_];
        for(auto &app : apps)
        {
            transitionChanges_.toTerminate_.push_back(app);                        
        }
        fn.startupConfigurations_[fn.currentState_].clear();
        fn.currentState_=fgs.fg_newState;
    }
    return true;
}

void ApplicationExecutionMgr::initialize()
{
    mkfifo("executablesFifo", 0777);
    mkfifo("smFifo", 0777);
    loadMachineConfigrations();
    loadExecutablesConfigrations();
    FunctionGroupState::Preconstruct("machineState", "startup");
    FunctionGroupState FGS(FunctionGroupState::Preconstruct("machineState", "startup"));
    setState(FGS);
    Execute();
    transitionChanges_.toStart_.clear();
    transitionChanges_.toTerminate_.clear();    
}

ApplicationExecutionMgr::ApplicationExecutionMgr(string rootPath)
{
    rootPath = rootPath;
}

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
    for (auto app : transitionChanges_.toTerminate_)
    {
        app->terminate();
        for (auto &ex : executables_)
        {
            for (int i=0  ; i< ex.startupConfigurations_.size();i++)
            if(app == &ex.startupConfigurations_[i])
            {
                ex.startupConfigurations_.erase(ex.startupConfigurations_.begin() +i);
            }
        }
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
