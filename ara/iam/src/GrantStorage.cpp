#include "../include/GrantStorage.hpp"

namespace pt = boost::property_tree;

std::map<std::string, std::vector<ara::iam::Grant>> ara::iam::GrantStorage::AccessMap;

void ara::iam::GrantStorage::ParseJson(std::string filePath)
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


    Json::Value access;
    ifstream f("iam_access.json");
    Json::Reader R;
    R.parse(f, access);
    Json::Value tmp;
    if (!access)
    {
        access["Cluster_name"] = "iam_json";
        access["Processes"] = Json::arrayValue;
        tmp["Process"] = Json::stringValue;
        tmp["info"]["GrantType"] = Json::stringValue;
        tmp["info"]["PR_Type"] = Json::stringValue;
        tmp["info"]["Service_ID"] = Json::stringValue;
        tmp["info"]["Instance_ID"] = Json::stringValue;
    }

    // Add To JSON FILE
    std::map<std::string, std::vector<ara::iam::Grant>>::iterator it = AccessMap.begin();
    while (it != AccessMap.end())
    {

        tmp["Process"] = Json::stringValue;
        tmp["info"]["GrantType"] = Json::stringValue;
        tmp["info"]["PR_Type"] = Json::stringValue;
        tmp["info"]["Service_ID"] = Json::stringValue;
        tmp["info"]["Instance_ID"] = Json::stringValue;

        tmp["Process"] = it->first;

        for(auto i = std::begin(it->second); i != std::end(it->second); ++i)
        {
            ara::iam::Grant GG = *i;
            tmp["info"]["GrantType"] = GG.GType;
            tmp["info"]["PR_Type"] = GG.PR_T;
            tmp["info"]["Service_ID"] = GG.S_id;
            tmp["info"]["Instance_ID"] = GG.In_id;

            access["Processes"].append(tmp);
        }        

        it++;
    }
    
    std::cout << access << std::endl;
    std::ofstream json_file("iam_access.json");
    json_file << access;
    json_file.close();

}

bool ara::iam::GrantStorage::SearchGrantStorage(const std::string P_name, const ara::iam::Grant & G)
{
    auto itr = ara::iam::GrantStorage::AccessMap.find(P_name);
    
    if (itr != AccessMap.end())
    {
        std::vector<ara::iam::Grant> GV = itr->second;
        for (auto & E : GV)
        {
            if (E.IsEqual(G))
            {
                return true;
            }
        }
    }
    return false;
}