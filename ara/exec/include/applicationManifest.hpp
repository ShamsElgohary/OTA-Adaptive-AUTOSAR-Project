#pragma once
#include <bits/stdc++.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace std;
namespace ara
{
    namespace exec
    {
        class ApplicationManifest 
        {

            public:
              typedef struct{
                    vector<string> dependency ;
                    vector<string> arguments ;
                    vector<string> environments ;
                    string scheduling_policy ;
                    string scheduling_priority ;
                    map<string,vector<string>>function_group_states ;
                    vector<string> machine_states ; 
            }startUpConfiguration; 

            string name;
            string executable_path;
            bool platform_application;
            vector<string> applicationStates;
            vector<startUpConfiguration> startUpConfigurations;
            ApplicationManifest(string manifestPath);
            
        };
    }
}

