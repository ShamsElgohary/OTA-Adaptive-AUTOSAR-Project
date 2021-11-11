#pragma once
#include <stdint.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <bits/stdc++.h>
#include "function_group_state.h"
namespace ara {
    namespace exec
    {
        class StateClient final
        {
            int fd;
            
        public:
            enum cases
            {
                faliure, sucess
            };
            bool setState(const FunctionGroupState &state);
            StateClient();
            ~StateClient();
        };
    }
}
   
