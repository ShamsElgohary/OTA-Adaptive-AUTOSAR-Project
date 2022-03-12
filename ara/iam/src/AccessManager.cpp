#include "../include/AccessManager.hpp"

std::map<std::string, std::vector<ara::iam::Grant>> ara::iam::AccessManager::AccessMap;

void ara::iam::AccessManager::ParseJson(std::string filePath)
{

}

bool ara::iam::AccessManager::InitGrantStorage(std::string basePath)
{
    ara::iam::AccessManager::ParseJson(std::string basePath);
    return 0;
}

bool ara::iam::AccessManager::InitServerAdapter(ara::iam::IPCserverInterface & server)
{

}

void ara::iam::AccessManager::RunEventLoop()
{

}