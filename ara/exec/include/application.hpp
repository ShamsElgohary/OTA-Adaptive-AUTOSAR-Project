#pragma once
#include <bits/stdc++.h>
#include "../include/applicationManifest.hpp"
#include <mutex>
#include <condition_variable>

using namespace std;
namespace ara 
{
    namespace exec 
    {
       
        class Application //process         
        {            
            public:
            enum class ProcessState : uint8_t 
            {
                Kidle,
                Krunning,
                Kterminate
            };

            typedef map<string,ProcessState> DependencyCollectionType;

            typedef struct{
                    ApplicationManifest::startUpConfiguration configration;
                    string name;
                    string executable_path; 
            }CtorToken; 
            int id;
            string name;
            string executable_path;
            ProcessState current_state ;
            string fifo_path_name;
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