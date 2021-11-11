#pragma once
#include <stdint.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <bits/stdc++.h>
#include "process.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace ara {
    namespace exec
    {
        enum class ExecutionState : uint8_t 
        {
            KRunning =0,
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
