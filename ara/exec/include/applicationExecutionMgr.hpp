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
#include <pthread.h>
#include <future>
#include <stdint.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include <bits/stdc++.h>
#include "execution_client.hpp"
#include"find_process_server.hpp"
#include"../../../utility/jsoncpp/header/json.h"
using namespace std;
namespace ara {
    namespace exec
    {
        typedef void * (*THREADFUNCPTR)(void *);
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
            private:
            string get_process_name(int pid);
            void* IAM_server(void* p);
            int smpipe{-1};
            public:
                vector<Application*> waiting;
                vector<Executable> executables_;
                unique_ptr<MachineManifest> manifest_ ;
                map<string,FunctionGroup*> function_groups_;
                const string rootPath ;
                PlatformStates platformState_ {PlatformStates::kRunning};
                TransitionChanges transitionChanges_;
                ApplicationExecutionMgr(string rootPath); 
                void initialize();
                bool run();
                bool setState(FunctionGroupState);
                bool loadMachineConfigrations(); 
                bool loadExecutablesConfigrations();
                bool ProcessStateClientRequest();
                bool Terminate();
                bool Execute();
                void IAM_handle();
        };
    }
}
