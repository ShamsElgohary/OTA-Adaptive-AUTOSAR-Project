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
                    map<string,string> dependency ;
                    map<string,vector<string>>function_group_states ;
                    vector<string> machine_states ; 
            }startUpConfiguration; 

            string name;
            string executable_path;
            bool platform_application;
            vector<startUpConfiguration> startUpConfigurations;
            ApplicationManifest(string manifestPath);
            
        };
    }
}

