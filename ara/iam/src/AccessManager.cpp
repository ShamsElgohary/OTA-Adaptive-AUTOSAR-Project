#include "../include/AccessManager.hpp"

void ara::iam::AccessManager::InitGrantStorage(std::string basePath)
{
    ara::iam::GrantStorage::ParseJson(basePath);
}

/* void ara::iam::AccessManager::InitServerAdapter(ara::iam::IPCserverInterface & server)
{
    // RUN SERVER


    // START LISTENING FOR GRANTS REQUESTS


} */

void ara::iam::AccessManager::RunEventLoop()
{
    // LISTEN FOR GRANTS REQUESTS


    // CREATE CLIENT SOCKET


    // RECIEVE PEER PID


    // RESOLVE PID FROM EM
    std::string P_name = "ucm";

    // RECIEVE GRANT FROM CLIENT 
    ara::iam::Grant G(1,2,"ComGrant", "Require");

    // SEARCH GRANTSTORAGE MAP
    bool rtn = ara::iam::GrantStorage::SearchGrantStorage(P_name, G);

    // RETURN RESULT TO CLIENT
    std::cout << "Result: " << rtn << std::endl;
}

