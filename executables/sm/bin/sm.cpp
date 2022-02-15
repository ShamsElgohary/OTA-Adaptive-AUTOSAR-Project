#include <iostream>
#include "execution_client.h"
#include "state_client.h"

using namespace ara::exec ;
int main ()
{
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::KRunning);
    StateClient x ;
    FunctionGroupState fgs(FunctionGroupState::Preconstruct("fn1", "play"));
    
    x.setState(fgs);

    std::cout<<"hello from excutable" ;
}