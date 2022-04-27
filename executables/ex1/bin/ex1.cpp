#include <iostream>
#include "execution_client.hpp"
#include "find_process_client.hpp"
#include<unistd.h>
using namespace std;
using namespace ara::exec ;

int main ()
{
    
    cout<<"hello from ex1 with pid = " <<getpid()<<"\n" ;
    usleep(1000000);
    
    ExecutionClient exec;

    exec.ReportExecutionStaste(ExecutionState::Krunning);

    FindProcessClient cl;
    cl.sendData(getpid());
    cout<<"ex1 "<<cl.receiveData()<<endl;
    while(1);
    return 0;

}