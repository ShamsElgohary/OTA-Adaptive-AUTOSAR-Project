#include <iostream>
#include "../../../ara/exec/include/execution_client.hpp"
using namespace std;
using namespace ara::exec ;
int main ()
{
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::Krunning);
    cout<<"hello from ex1" ;
    return 0;
}