#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <bits/stdc++.h>
#include "../include/application.hpp"

using namespace std;

namespace ara {
    namespace exec
    {
        class FunctionGroup
        {
            public:
            
            const string name_;
            const vector <string> allStates_ ;
            const string* currentState_;
            //map<string,set<Application*>> startupConfigurations_;

            typedef struct {
                int fg_index ; 
                int size ;
                vector<string> fg_names;
                map<string , vector<string>> fg_states;
            } CtorToken;


            static FunctionGroup::CtorToken Preconstruct(string machine_manifest_path);
            FunctionGroup(FunctionGroup::CtorToken &&token);
            ~FunctionGroup();
        };
    }
}
