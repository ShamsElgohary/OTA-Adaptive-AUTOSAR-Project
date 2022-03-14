#include "../include/AccessManager.hpp"

int main (void)
{
    ara::iam::AccessManager::InitGrantStorage("../../etc/system/iam/access_control_lists.json");
    ara::iam::AccessManager::InitServerAdapter();
    ara::iam::AccessManager::RunEventLoop();

    std::cout << "Server End" << std::endl;

    return 0;
}