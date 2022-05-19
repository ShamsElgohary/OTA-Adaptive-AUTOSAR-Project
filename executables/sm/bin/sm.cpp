#include <iostream>
#include<unistd.h>
#include "../../../ara/exec/include/execution_client.hpp"
#include "../../../ara/exec/include/state_client.hpp"

using namespace ara::exec ;
int main ()
{
    std::cout<<"[SM] hello from sm with pid = " <<getpid()<<"\n" ;
    ExecutionClient y;
    y.ReportExecutionStaste(ExecutionState::Krunning);
sleep(4);
    StateClient x ;
    FunctionGroupState fgs0(FunctionGroupState::Preconstruct("machineFG", "running"));
    x.setState(fgs0);
sleep(4);
    FunctionGroupState fgs1(FunctionGroupState::Preconstruct("fn1", "idle"));
    x.setState(fgs1);
sleep(5);
    FunctionGroupState fgs2(FunctionGroupState::Preconstruct("fn1", "play"));
    x.setState(fgs2);
sleep(5);

    FunctionGroupState fgs5(FunctionGroupState::Preconstruct("fn1", "terminate"));
    x.setState(fgs5);
sleep(5);
    FunctionGroupState fgs8(FunctionGroupState::Preconstruct("fn3", "run"));
    x.setState(fgs8);
    sleep(4);

    FunctionGroupState fgs9(FunctionGroupState::Preconstruct("fn3", "end"));
    x.setState(fgs9);
sleep(4);
    FunctionGroupState fgs10(FunctionGroupState::Preconstruct("machineFG", "sleep"));
    x.setState(fgs10);
sleep(4);
    //while(1);
}