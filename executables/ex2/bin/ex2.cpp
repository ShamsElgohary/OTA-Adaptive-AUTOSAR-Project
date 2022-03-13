#include <iostream>
#include "../../../ara/exec/include/execution_client.hpp"
#include<unistd.h>

using namespace ara::exec ;
int main ()
{
    std::cout<<"hello from ex2\n" ;
    usleep(1000000);
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::Krunning);
}