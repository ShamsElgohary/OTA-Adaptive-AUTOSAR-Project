#pragma once
#include <bits/stdc++.h>
#include "applicationManifest.hpp"
#include "execution_client.hpp"
#include "../../../utility/general.hpp"
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
            void * parent{nullptr};
            pid_t id{0};
            int fd;
            string name;
            string executable_path;
            ExecutionState current_state;
            mutex mur;
            condition_variable condr;
            condition_variable condt;
            map<string, Application *> depend;
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