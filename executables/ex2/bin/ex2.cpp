#include <iostream>
#include "execution_client.hpp"
#include<unistd.h>

using namespace ara::exec ;
int main ()
{
    std::cout<<"[Ex2] hello from ex2 with pid = " <<getpid()<<"\n"  ;
    usleep(1000000);
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::Krunning);
    while(1);
}