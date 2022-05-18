#include <iostream>
#include "execution_client.hpp"
#include <unistd.h>
using namespace std;
using namespace ara::exec;
void handle_sigTerm(int sig)
{
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::Kterminate);
    exit(1);
}
int main()
{
    struct sigaction sa;
    sa.sa_flags=SA_RESTART;
    sa.sa_handler =handle_sigTerm ;
    sigaction(SIGTERM,&sa,NULL);
    cout << "[Ex1] hello from ex1 with pid = " << getpid() << "\n";
    sleep(3);

    ExecutionClient exec;

    exec.ReportExecutionStaste(ExecutionState::Krunning);
    sleep(3);
    exec.ReportExecutionStaste(ExecutionState::Kterminate);

    return 0;
}