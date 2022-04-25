#include <iostream>
#include "../../../ara/exec/include/execution_client.hpp"
#include<unistd.h>
using namespace std;
using namespace ara::exec ;

int main ()
{
    
    cout<<"hello from ex1\n" ;
    usleep(1000000);
    
    ExecutionClient exec;

    exec.ReportExecutionStaste(ExecutionState::Krunning);
    return 0;

}