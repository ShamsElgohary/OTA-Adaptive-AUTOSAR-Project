#pragma once
#include <bits/stdc++.h>
#include "application.hpp"
#include "applicationManifest.hpp"
#include "machineManifest.hpp"
#include "function_group_state.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "function_group.hpp"
#include "find_process_server.hpp"
#include "../../../utility/jsoncpp/header/json.h"
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
#include "simulation.hpp"
#include "../../../utility/general.hpp"
#include <thread>
#include <fstream>
#include <algorithm>

using namespace std;

namespace ara
{
    namespace exec
    {

        class ApplicationExecutionMgr
        {
            int smpipe{-1};
            int fd1;

        public:
            typedef struct
            {
                ApplicationManifest manifest_;
                vector<Application *> startupConfigurations_;
            } Executable;

            typedef struct
            {
                vector<Application *> toStart_;
                vector<Application *> toTerminate_;
            } TransitionChanges;

            mutex mu;
            future<void> iam_future;
            FunctionGroupState newfunctionGroup;
            vector<Executable> executables_;
            unique_ptr<MachineManifest> manifest_;
            map<string, FunctionGroup *> function_groups_;
            map<string, string> excutables_map;//{name,path}
            const string rootPath;
            bool debugging_mode;
            TransitionChanges transitionChanges_;
            simulation sim_socket{8088};
            ApplicationExecutionMgr(string rootPath);
            void initialize();
            bool run();
            bool setState(FunctionGroupState);
            bool loadMachineConfigrations();
            bool loadExecutablesConfigrations();
            bool ProcessStateClientRequest();
            future<void> updateProcessState();
            bool Terminate();
            bool Execute();
            string get_process_name(int test_id);
            future<void> IAM_handle();
            void reportConfig_simulation();
            void report_success_sm();
            void wait_for_gui();
            void reparse();
        };
    }
}
