#include <iostream>
#include "execution_client.h"
using namespace ara::exec ;
int main ()
{
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::KRunning);
    
    std::cout<<"hello from excutable" ;
}