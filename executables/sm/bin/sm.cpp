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
    usleep(5*1000000);
    StateClient x ;
    FunctionGroupState fgs0(FunctionGroupState::Preconstruct("machineFG", "running"));
    x.setState(fgs0);
    usleep(5*1000000);
    FunctionGroupState fgs1(FunctionGroupState::Preconstruct("fn1", "idle"));
    x.setState(fgs1);
     usleep(5*1000000);
    FunctionGroupState fgs2(FunctionGroupState::Preconstruct("fn1", "play"));
    x.setState(fgs2);
    usleep(5*1000000);
    FunctionGroupState fgs3(FunctionGroupState::Preconstruct("fn2", "idle"));
    x.setState(fgs3);
    usleep(5*1000000);
    FunctionGroupState fgs4(FunctionGroupState::Preconstruct("fn2", "run"));
    x.setState(fgs4);
    usleep(5*1000000);
    FunctionGroupState fgs5(FunctionGroupState::Preconstruct("fn1", "terminate"));
    x.setState(fgs5);
    usleep(5*1000000);
    FunctionGroupState fgs6(FunctionGroupState::Preconstruct("fn2", "end"));
    x.setState(fgs6);
    usleep(5*1000000);
    FunctionGroupState fgs7(FunctionGroupState::Preconstruct("fn3", "idle"));
    x.setState(fgs7);
    usleep(5*1000000);
    FunctionGroupState fgs8(FunctionGroupState::Preconstruct("fn3", "run"));
    x.setState(fgs8);
    usleep(5*1000000);
    usleep(5*1000000);
    usleep(5*1000000);
    usleep(5*1000000);

    FunctionGroupState fgs9(FunctionGroupState::Preconstruct("fn3", "end"));
    x.setState(fgs9);
    usleep(5*1000000);
    FunctionGroupState fgs10(FunctionGroupState::Preconstruct("machineFG", "sleep"));
    x.setState(fgs10);
    usleep(5*1000000);
    while(1);
}