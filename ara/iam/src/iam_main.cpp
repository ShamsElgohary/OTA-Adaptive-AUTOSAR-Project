#include "../include/AccessManager.hpp"
#include "execution_client.hpp"
int main (void)
{
    ara::iam::AccessManager::InitGrantStorage("/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/ara/etc/system/iam/access_control_lists.json");
    ara::iam::AccessManager::InitServerAdapter();
    ara::exec::ExecutionClient exec;
    exec.ReportExecutionStaste(ara::exec::ExecutionState::Krunning);
    ara::iam::AccessManager::RunEventLoop();

    std::cout << "Server End" << std::endl;

    return 0;
}