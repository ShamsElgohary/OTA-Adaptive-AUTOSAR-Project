#pragma once
#include <bits/stdc++.h>
#include "application.hpp"
#include "applicationManifest.hpp"
#include "machineManifest.hpp"
#include "function_group_state.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "function_group.hpp"
#include <thread>
#include <future>
#include <stdint.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <boost/filesystem.hpp>
#include <bits/stdc++.h>
#include "execution_client.hpp"
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
        }TransitionChanges;

        class ApplicationExecutionMgr final
        {
                string smFifo;
            public:
                vector<Executable> executables_;
                //set<Application*> executedStartupConfigurations_;
                unique_ptr<MachineManifest> manifest_ ;
                const string rootPath ;
                map<string,FunctionGroup> functionGroups_;
                PlatformStates platformState_ {PlatformStates::kRunning};
                TransitionChanges transitionChanges_;
                //ApplicationExecutionMgr();
                ApplicationExecutionMgr(string rootPath); //highlevel : calls functions only
                void initialize();//done
                bool run();//done
                bool setState(FunctionGroupState);//done
                bool loadMachineConfigrations(); //done
                bool loadExecutablesConfigrations();//done
                bool ProcessExecutionStateReport();//done
                bool ProcessStateClientRequest();//done
                void Terminate();//done
                void Execute();//done

        };
    }
}
