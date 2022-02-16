#include "../include/applicationExecutionMgr.hpp"

using namespace std;
using namespace ara::exec;
using namespace boost::filesystem;

//1-parses directories to reach execution manifests
//2-makes executable objects and passes its manifest path to its constructor
//3-push executable objects to executables_ vector
bool ApplicationExecutionMgr::loadExecutablesConfigrations()
{
    path p(string(rootPath) + "/executables");
    try
    {
        if (exists(p))
        {
            if (is_directory(p))
            {
                cout << p << " is a directory containing:\n";

                for (directory_entry &x : directory_iterator(p))
                {
                    string p = x.path().string() + "/etc" + x.path().filename().string() + ".json";
                    Executable E = {ApplicationManifest(p)};
                    for (auto &c : E.manifest_.startUpConfigurations)
                    {
                        E.startupConfigurations_.push_back(Application(c,E.manifest_.name,E.manifest_.executable_path));
                    }
                    executables_.push_back(E);
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
    manifest_ = make_unique<MachineManifest>(string(rootPath) + "/etc/system/machine_manifest.json");
    return true;
}

bool ApplicationExecutionMgr::setState(FunctionGroupState fgs)
{
    for (auto &ex : executables_)
    {
        for (auto &app : ex.startupConfigurations_)
        {
            if (fgs.fg_name != "machineState")
            {
                for (auto &state : app.configuration_->function_group_states[fgs.fg_name])
                {
                    if (state == fgs.fg_newState)
                    {
                        transitionChanges_.toStart_.push_back(&app);
                        break;

                    }
                }
            }
            else
            {
                for (auto &state : app.configuration_->machine_states)
                {
                    if (state == fgs.fg_newState)
                    {
                        transitionChanges_.toStart_.push_back(&app);
                        break;
                    }
                }
            }
            if (app.current_state == Application::ProcessState::Krunning)
            {
                bool flag =false;
                if (fgs.fg_name != "machineState")
                {
                    for (auto &state : app.configuration_->function_group_states[fgs.fg_name])
                    {
                        if (state == fgs.fg_newState)
                        {
                            flag=true;
                            break;
                        }
                    }
                }
                else
                {
                    for (auto &state : app.configuration_->machine_states)
                    {
                        if (state == fgs.fg_newState)
                        {
                            flag=true;
                            break;
                        }
                    }
                }
                if(flag)
                    transitionChanges_.toTerminate_.push_back(&app);
            }
        }
    }
    Terminate();
    Execute();
    ProcessExecutionStateReport();
}

void ApplicationExecutionMgr::initialize()
{
    loadMachineConfigrations();
    loadExecutablesConfigrations();
    FunctionGroupState::Preconstruct("MachineFG", "startup");
    FunctionGroupState FGS(FunctionGroupState::Preconstruct("MachineFG", "startup"));
    setState(FGS);
}

ApplicationExecutionMgr::ApplicationExecutionMgr(string rootPath)
{
    //sets the value of rootPath (member variable in the class) to the value of the input rootPath
    rootPath = rootPath;
    
}

bool ApplicationExecutionMgr::run()
{
    //while
}

void ApplicationExecutionMgr::Terminate()
{
    for(auto app : transitionChanges_.toTerminate_)
    {
        app->terminate() ;
    }
}
void ApplicationExecutionMgr::Execute()
{
    for(auto app : transitionChanges_.toStart_)
    {
        app->start() ;
    }
}

