#include "../include/AccessManager.hpp"


void ara::iam::AccessManager::InitGrantStorage(std::string basePath)
{
    ara::iam::GrantStorage::ParseJson(basePath);
}

std::uint8_t ara::iam::AccessManager::InitServerAdapter()
{
    // RUN SERVER
    return server.ServerSocketInit();
}



void ara::iam::AccessManager::RunEventLoop()
{
    while (true)
    {
        // LISTEN FOR GRANTS REQUESTS
        int sd = server.Listen();

        // Receive PID
        int PID = server.getPeerId(sd);

        std::cout << "[RECEIVED] " << PID << std::endl;


        // Send PID to EM
        FPC.sendData(PID);

        // Receive Proc
        std::string P_name = FPC.receiveData();
        cout<<"IAM: process name "<<P_name<<endl;
        // RECIEVE GRANT FROM CLIENT 
        ara::iam::Grant G = server.Receive(sd);

        // SEARCH GRANTSTORAGE MAP
        bool rtn = ara::iam::GrantStorage::SearchGrantStorage(P_name, G);

        std::cout << "Result: " << rtn << std::endl;

        // RETURN RESULT TO CLIENT
        server.Send(rtn, sd);
    }
}

