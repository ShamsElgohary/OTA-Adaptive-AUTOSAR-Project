#pragma once
#include <bits/stdc++.h>
#include "applicationManifest.hpp"
#include "execution_client.hpp"

using namespace std;
namespace ara
{
    namespace exec
    {

        class Application
        {
            typedef struct
            {
                ApplicationManifest::startUpConfiguration configration;
                string name;
                string executable_path;
            } CtorToken;

        public:
            pid_t id{NULL};
            int fd;
            string name;
            string executable_path;
            ExecutionState current_state;
            ApplicationManifest::startUpConfiguration configuration_;
            Application(Application::CtorToken &&token);
            Application(ApplicationManifest::startUpConfiguration con, string name, string path);
            ~Application();
            void start();
            void terminate();
            void Update_status();
        };
    }
}