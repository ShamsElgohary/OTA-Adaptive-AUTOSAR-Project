#include "../include/AccessManager.hpp"

namespace pt = boost::property_tree;



std::map<std::string, std::vector<ara::iam::Grant>> ara::iam::AccessManager::GrantStorage;

void ara::iam::AccessManager::ParseJson(std::string filePath)
{
    // Create a root
    pt::ptree root;
    // Load the json file in this ptree
    pt::read_json(filePath, root);
    for(auto&& p : root.get_child("Grants"))
    {
        const pt::ptree & subtree = p.second;
        ara::iam::Process_Name P_Name = subtree.get<std::string>("Process");
        std::vector<ara::iam::Grant> VG;
        for(auto&& e : subtree.get_child("Elements"))
        {
            const pt::ptree & sub = e.second;
            ara::iam::Grant_Type GT = sub.get<ara::iam::Grant_Type>("GrantType");
            ara::iam::PR_Type PRT = sub.get<ara::iam::PR_Type>("PR_Type");
            ara::iam::ServiceID S_ID = sub.get<ara::iam::ServiceID>("ServiceID");
            ara::iam::InstanceID I_ID = sub.get<ara::iam::InstanceID>("InstanceID");

            ara::iam::Grant G(S_ID , I_ID, GT, PRT);
            VG.push_back(G);
        }
        AccessMap.insert({P_Name, VG});
    }
}

bool ara::iam::AccessManager::InitGrantStorage(std::string basePath)
{
    ara::iam::AccessManager::ParseJson(basePath);
    return 0;
}

//bool ara::iam::AccessManager::InitServerAdapter(ara::iam::IPCserverInterface & server)
//{

//}

void ara::iam::AccessManager::RunEventLoop()
{

}

