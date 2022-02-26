#include <iostream>
#include "../../../ara/exec/include/execution_client.hpp"
using namespace ara::exec ;
int main ()
{
    std::cout<<"hello from ex2\n" ;
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::Krunning);
}