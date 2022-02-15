#pragma once
#include <bits/stdc++.h>
#include "application.hpp"
#include "applicationManifest.hpp"
#include "machineManifest.hpp"
#include "function_group_state.hpp"
#include"applicationDependencyOrdering.hpp"
using namespace std;
namespace ara {
    namespace exec
    {
        enum class PlatformStates : uint8_t
        {
            kRunning,
            kRestarting,
            kShutdown
        };
       
        typedef struct{
            ApplicationManifest manifest_;
            vector<Application> startupConfigurations_;
        }Executable;

        typedef struct{
            vector<Application*> toStart_;
            vector<Application*> toTerminate_;
        }TransationChanges;

        class ApplicationExecutionMgr final
        {
            public:
                vector<Executable> executables_;
                set<Application*> executedStartupConfigurations_;
                unique_ptr<MachineManifest> manifest_ ;
                const string rootPath ;
                map<string,FunctionGroup> functionGroups_;
                PlatformStates platformState_;
                TransationChanges transitionChanges_;
                //ApplicationExecutionMgr();
                ApplicationExecutionMgr(string rootPath); //highlevel : calls functions only
                void initialize();//done
                bool run();
                bool setState(FunctionGroupState);
                bool loadMachineConfigrations(); //done
                bool loadExecutablesConfigrations();//done
                bool ProcessExecutionStateResponce();
                bool ProcessStateClientRequest();
                TransationChanges calculateTransition();
                bool removeTerminatedProcesses();
                ~ApplicationExecutionMgr();
        };
    }
}
