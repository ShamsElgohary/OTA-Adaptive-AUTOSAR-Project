#pragma once
#include <bits/stdc++.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace std;
namespace ara
{
    namespace exec
    {
      
        class Executable 
        {

            public:
              typedef struct{
                    vector<string> dependency ;
                    vector<string> arguments ;
                    vector<string> environments ;
                    string scheduling_policy ;
                    string scheduling_priority ;
            }startUpConfigration; 

            string name;
            string executable_path;
            bool platform_application;
            
            map<pair<string,string>,startUpConfigration> configration  ;
            Executable(string manifestPath);
        };
    }
}

