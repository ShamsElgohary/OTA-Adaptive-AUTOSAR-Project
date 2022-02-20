#include <iostream>
#include "../../../ara/exec/include/execution_client.hpp"
using namespace ara::exec ;
int main ()
{
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::KRunning);
    std::cout<<"hello from ex2" ;
}