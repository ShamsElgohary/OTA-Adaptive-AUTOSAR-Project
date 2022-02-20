#include <iostream>
#include "../../../ara/exec/include/execution_client.hpp"
using namespace ara::exec ;
int main ()
{
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::Krunning);
    std::cout<<"hello from ex2" ;
}