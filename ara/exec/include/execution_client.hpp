#pragma once
#include <stdint.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <bits/stdc++.h>
#include "application.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace ara {
    namespace exec
    {
        enum class ExecutionState : char 
        {
            Kidle=0,
            Krunning=1,
            Kterminate=2
        };
        
        class ExecutionClient final
        {
        
        int fd;
        public:
        

            void ReportExecutionStaste(ExecutionState state);
            ExecutionClient();
            ~ExecutionClient();
        };
    }
}
