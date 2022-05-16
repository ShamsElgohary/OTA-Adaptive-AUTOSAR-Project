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
    sleep(1);
    StateClient x ;
    FunctionGroupState fgs0(FunctionGroupState::Preconstruct("machineFG", "running"));
    x.setState(fgs0);
    sleep(1);
    FunctionGroupState fgs1(FunctionGroupState::Preconstruct("fn1", "idle"));
    x.setState(fgs1);
    sleep(1);
    FunctionGroupState fgs2(FunctionGroupState::Preconstruct("fn1", "play"));
    x.setState(fgs2);
    sleep(1);
    FunctionGroupState fgs5(FunctionGroupState::Preconstruct("fn1", "terminate"));
    x.setState(fgs5);
    sleep(1);
    
    FunctionGroupState fgs8(FunctionGroupState::Preconstruct("fn3", "run"));
    x.setState(fgs8);
    sleep(1);


    FunctionGroupState fgs9(FunctionGroupState::Preconstruct("fn3", "end"));
    x.setState(fgs9);
    sleep(1);
    FunctionGroupState fgs10(FunctionGroupState::Preconstruct("machineFG", "sleep"));
    x.setState(fgs10);
    sleep(1);
    while(1);
}