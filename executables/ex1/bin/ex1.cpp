#include <iostream>
#include "../../../ara/exec/include/execution_client.hpp"
using namespace std;
using namespace ara::exec ;
int main ()
{
    
    cout<<"hello from ex1\n" ;
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::Krunning);
    return 0;
}