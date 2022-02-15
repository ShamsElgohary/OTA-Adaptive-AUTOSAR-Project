#include "../include/applicationExecutionMgr.hpp"

using namespace std;
using namespace ara::exec;

//1-parses directories to reach execution manifests
//2-makes executable objects and passes its manifest path to its constructor
//3-push executable objects to executables_ vector
bool ApplicationExecutionMgr::loadExecutablesConfigrations()
{
    path p(string(ARA_ROOT) + "/executables");
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
                        E.startupConfigurations_.push_back(Application(&c));
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
    manifest_ = new MachineManifest(string(rootPath) + "/etc/system/machine_manifest.json");
    return true;
}

bool ApplicationExecutionMgr::setState(FunctionGroupState fgs)
{
    for (auto &ex : Executable)
    {
        for (auto &app : Executable.startupConfigurations_)
        {
            if (fgs.fg_name != "machineState")
            {
                for (auto &state : app.configuration_->function_group_states[fgs.fg_name])
                {
                    if (state == fgs.fg_newState)
                    {
                        transitionChanges_.toStart_.push_back(&app);
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
                    }
                }
            }
            if (app.current_state == ProcessState::Krunning)
            {
                if (fgs.fg_name != "machineState")
                {
                    for (auto &state : app.configuration_->function_group_states[fgs.fg_name])
                    {
                        if (state == fgs.fg_newState)
                        {
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
                            break;
                        }
                    }
                }

                //transitionChanges_.toTerminate_.push_back(&app);
            }
        }
    }
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

void Terminate(startupConfigurations_)
{
    
}
