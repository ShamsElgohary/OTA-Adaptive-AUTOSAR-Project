#include "../include/types.hpp"
#include "../include/Grant.hpp"
namespace pt = boost::property_tree;


int main (void)
{
    // Create a root
    pt::ptree root;
    // Load the json file in this ptree
    pt::read_json("/home/yasmin/Desktop/Graduation_Project/02-OurImpement/OTA-Adaptive-AUTOSAR-Project/ara/iam/GrantStorage.json", root);
    for(auto&& p : root.get_child("Grants"))
    {
        const pt::ptree & subtree = p.second;
        ara::iam::Process_Name P_Name = subtree.get<std::string>("Process");
        for(auto&& e : subtree.get_child("Elements"))
        {
            const pt::ptree & sub = e.second;
            ara::iam::Grant_Type GT = sub.get<ara::iam::Grant_Type>("GrantType");
            ara::iam::PR_Type PRT = sub.get<ara::iam::PR_Type>("PR_Type");
            ara::iam::ServiceID S_ID = sub.get<ara::iam::ServiceID>("ServiceID");
            ara::iam::InstanceID I_ID = sub.get<ara::iam::InstanceID>("InstanceID");

            ara::iam::Grant G(S_ID , I_ID, GT, PRT);
        }
    }
    std::string filePath = "/home/yasmin/Desktop/Graduation_Project/02-OurImpement/OTA-Adaptive-AUTOSAR-Project/ara/iam/GrantStorage.json";
    //ara::iam::AccessManager::InitGrantStorage(filePath);
    return 0;
}