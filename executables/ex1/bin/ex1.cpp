#include <iostream>
#include "execution_client.hpp"
#include<unistd.h>
using namespace std;
using namespace ara::exec ;

int main ()
{
    
    cout<<"[Ex1] hello from ex1 with pid = " <<getpid()<<"\n" ;
    usleep(1000000);
    
    ExecutionClient exec;

    exec.ReportExecutionStaste(ExecutionState::Krunning);

    while(1);
    return 0;

}