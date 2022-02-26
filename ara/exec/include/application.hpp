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
            typedef struct{
                    ApplicationManifest::startUpConfiguration configration;
                    string name;
                    string executable_path; 
            }CtorToken; 
            public:
                int id;
                int fd;
                string name;
                string executable_path;
                ExecutionState current_state ;
                ApplicationManifest::startUpConfiguration configuration_;
                static Application::CtorToken preconstruct(ApplicationManifest &ex,string fg_name,string fg_state);
                Application(Application::CtorToken && token);
                Application(ApplicationManifest::startUpConfiguration con, string name , string path);
                int start();
                void terminate();
                void Update_status();
        };
    }
}