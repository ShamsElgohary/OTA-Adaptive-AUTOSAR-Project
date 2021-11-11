#pragma once
#include <bits/stdc++.h>
#include "executable.h"
using namespace std;
namespace ara 
{
    namespace exec 
    {
       
        class Process
        {            
            public:
            enum class processState : uint8_t 
            {
                Kidel,
                Krunning,
                Kterminate
            };

            typedef struct{
                    Executable::startUpConfigration configration;
                    string name;
                    string executable_path;
                    string function_group_name;
                    string function_group_state; 
            }CtorToken; 
            
            int id;
            string name;
            string executable_path;
            string function_group_name;
            string function_group_state;
            processState current_state ;
            Executable::startUpConfigration configration;
            static Process::CtorToken preconstruct(Executable &ex,string fng,string state);
            Process(Process::CtorToken && token);
            void update_state();
            int run();
            void terminate();
        };
    }
}