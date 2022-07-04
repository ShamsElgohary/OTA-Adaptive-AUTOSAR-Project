#include "../include/AccessManager.hpp"
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
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handle_sigTerm;
    sigaction(SIGTERM, &sa, NULL);

    ara::exec::ExecutionClient exec;
    exec.ReportExecutionStaste(ara::exec::ExecutionState::Krunning);
    sleep(1);

    ara::iam::AccessManager iam;
    iam.InitGrantStorage(CUSTOMIZED_PROJECT_PATH + "executables/etc/system/iam/access_control_lists.json");
    iam.InitServerAdapter();
    iam.RunEventLoop();

    return 0;
}