#include "../include/AccessManager.hpp"
#include "execution_client.hpp"
int main(void)
{
    ara::iam::AccessManager iam;
    iam.InitGrantStorage(CUSTOMIZED_PROJECT_PATH + "/executables/etc/system/iam/access_control_lists.json");
    iam.InitServerAdapter();
    cout << "2" << endl;

    ara::exec::ExecutionClient exec;
    cout << "3" << endl;

    exec.ReportExecutionStaste(ara::exec::ExecutionState::Krunning);
    cout << "4" << endl;

    iam.RunEventLoop();
    cout << "5" << endl;

    std::cout << "[iamServer] Server End" << std::endl;

    return 0;
}