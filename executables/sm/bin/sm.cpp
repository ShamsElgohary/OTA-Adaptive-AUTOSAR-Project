#include <iostream>
#include "../../../ara/exec/include/execution_client.hpp"
#include "../../../ara/exec/include/state_client.hpp"

using namespace ara::exec ;
int main ()
{
    std::cout<<"hello from SM\n" ;
    ExecutionClient y;
    y.ReportExecutionStaste(ExecutionState::Krunning);
    StateClient x ;
    FunctionGroupState fgs1(FunctionGroupState::Preconstruct("fn1", "play"));
    x.setState(fgs1);
    FunctionGroupState fgs2(FunctionGroupState::Preconstruct("fn2", "run"));
    x.setState(fgs2);
    FunctionGroupState fgs3(FunctionGroupState::Preconstruct("fn1", "terminate"));
    x.setState(fgs3);
    FunctionGroupState fgs4(FunctionGroupState::Preconstruct("fn2", "end"));
    x.setState(fgs4);

}