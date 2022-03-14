#include "../include/AccessManager.hpp"

ara::iam::IPCserverInterface ara::iam::AccessManager::server;

void ara::iam::AccessManager::InitGrantStorage(std::string basePath)
{
    ara::iam::GrantStorage::ParseJson(basePath);
}

std::uint8_t ara::iam::AccessManager::InitServerAdapter()
{
    // RUN SERVER
    return ara::iam::AccessManager::server.ServerSocketInit();
}

void ara::iam::AccessManager::RunEventLoop()
{
    while (1)
    {
        // LISTEN FOR GRANTS REQUESTS
        int sd = ara::iam::AccessManager::server.Listen();

        // Receive PID


        // RECIEVE PEER PID


        // RESOLVE PID FROM EM
        std::string P_name = "ucm";

        // RECIEVE GRANT FROM CLIENT 
        ara::iam::Grant G = ara::iam::AccessManager::server.Receive(sd);

        // SEARCH GRANTSTORAGE MAP
        bool rtn = ara::iam::GrantStorage::SearchGrantStorage(P_name, G);

        std::cout << "Result: " << rtn << std::endl;

        // RETURN RESULT TO CLIENT
        ara::iam::AccessManager::server.Send(rtn, sd);
    }
}

