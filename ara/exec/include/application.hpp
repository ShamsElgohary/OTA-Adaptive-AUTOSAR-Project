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
       
        class Application
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
            DependencyCollectionType depends_;
            ApplicationManifest::startUpConfiguration *configuration_;
            static Application::CtorToken preconstruct(ApplicationManifest &ex);
            Application(Application::CtorToken && token);
            void check_for_state(Application::ProcessState s);
            void update_state();
            int start();
            void terminate();
        };
    }
}