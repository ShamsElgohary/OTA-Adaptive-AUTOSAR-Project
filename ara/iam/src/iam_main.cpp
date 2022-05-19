#include "../include/AccessManager.hpp"
#include "execution_client.hpp"
int main(void)
{
    ara::iam::AccessManager iam;
    iam.InitGrantStorage(CUSTOMIZED_PROJECT_PATH + "/executables/etc/system/iam/access_control_lists.json");
    iam.InitServerAdapter();

    ara::exec::ExecutionClient exec;

    exec.ReportExecutionStaste(ara::exec::ExecutionState::Krunning);

    iam.RunEventLoop();


    return 0;
}