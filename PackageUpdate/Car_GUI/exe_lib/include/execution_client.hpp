#pragma once
#include <stdint.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <bits/stdc++.h>

namespace ara {
    namespace exec
    {
        enum class ExecutionState : char 
        {
            Kidle,
            Krunning,
            Kterminate
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
