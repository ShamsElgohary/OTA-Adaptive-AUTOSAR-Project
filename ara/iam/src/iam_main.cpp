#include "../include/AccessManager.hpp"
#include "execution_client.hpp"
int main (void)
{
    ara::iam::AccessManager iam;
    iam.InitGrantStorage("/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/executables/etc/system/iam/access_control_lists.json");
    iam.InitServerAdapter();
    ara::exec::ExecutionClient exec;
    exec.ReportExecutionStaste(ara::exec::ExecutionState::Krunning);
    iam.RunEventLoop();

    std::cout << "[iamServer] Server End" << std::endl;

    return 0;
}