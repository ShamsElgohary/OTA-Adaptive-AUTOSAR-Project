#include <iostream>
#include "execution_client.hpp"
#include "find_process_client.hpp"
#include<unistd.h>
using namespace std;
using namespace ara::exec ;

int main ()
{
    
    cout<<"hello from ex1\n" ;
    usleep(1000000);
    
    ExecutionClient exec;

    exec.ReportExecutionStaste(ExecutionState::Krunning);

    FindProcessClient cl;
    cl.sendData(getpid());
    cout<<"ex1 "<<cl.receiveData()<<endl;
    usleep(5*1000000);


    cl.sendData(getpid());
    cout<<"ex1 2 "<<cl.receiveData()<<endl;
    return 0;

}