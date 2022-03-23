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
    while (true)
    {
        // LISTEN FOR GRANTS REQUESTS
        int sd = ara::iam::AccessManager::server.Listen();

        // Receive PID
        int PID = ara::iam::AccessManager::server.getPeerId(sd);

        std::cout << "[RECEIVED] " << PID << std::endl;

        // RESOLVE PID FROM EM
        //ara::em::FindProcessClient FPC;

        // Send PID to EM
        // FPC.sendData(PID);

        // Receive Proc
        std::string P_name = "ucm";
        //P_name = FPC.receiveData();

        // RECIEVE GRANT FROM CLIENT 
        ara::iam::Grant G = ara::iam::AccessManager::server.Receive(sd);

        // SEARCH GRANTSTORAGE MAP
        bool rtn = ara::iam::GrantStorage::SearchGrantStorage(P_name, G);

        std::cout << "Result: " << rtn << std::endl;

        // RETURN RESULT TO CLIENT
        ara::iam::AccessManager::server.Send(rtn, sd);
    }
}

