#include <iostream>
#include "execution_client.hpp"
#include <unistd.h>
using namespace std;
using namespace ara::exec;

int main()
{

    cout << "[Ex1] hello from ex1 with pid = " << getpid() << "\n";
    usleep(100000);

    ExecutionClient exec;

    exec.ReportExecutionStaste(ExecutionState::Krunning);

    exec.ReportExecutionStaste(ExecutionState::Kterminate);

    return 0;
}