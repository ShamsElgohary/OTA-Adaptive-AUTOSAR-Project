#include "../include/servicestorage.hpp"

using namespace someip;

void servicestorage::AddToServiceRegistry(uint16_t Service_ID, serviceinfo serviceinformation)
{
    auto itr = servicemap.find(Service_ID);
    if (itr == servicemap.end())
    {
        std::vector<serviceinfo> v1;
        v1.push_back(serviceinformation);
        servicemap.insert(std::make_pair(Service_ID, v1));
    }
    else
    {
        for (auto it = itr->second.begin(); it != itr->second.end(); ++it)
        {
            if (it->Instance_ID == serviceinformation.Instance_ID)
            {
                // error
                return;
            }
        }
        itr->second.push_back(serviceinformation);
    }
}

void servicestorage::RemoveService(uint16_t Service_ID, uint16_t instance_id)
{
    auto itr = servicemap.find(Service_ID);
    if (itr == servicemap.end())
    {
        // error
    }
    else
    {
        for (auto it = itr->second.begin(); it != itr->second.end(); ++it)
        {
            if (it->Instance_ID == instance_id)
            {
                itr->second.erase(it);

                if (itr->second.empty())
                {
                    servicemap.erase(itr->first);
                }
                return;
            }
        }
    }
}

std::vector<serviceinfo> servicestorage::SearchServiceRegistry(uint16_t Service_ID, uint16_t Instance_ID)
{
    std::vector<serviceinfo> rtn;
    auto itr = servicemap.find(Service_ID);
    std::vector<serviceinfo> v1;
    if (Instance_ID != 0xFFFF)
    {

        for (auto it = itr->second.begin(); it != itr->second.end(); ++it)
        {
            if (it->Instance_ID == Instance_ID)
            {
                v1.push_back(*it);
            }
        }
        return v1;
    }
    else
    {
        return itr->second;
    }
}

serviceinfo servicestorage::SetServiceInfo(uint16_t Instance_ID, std::string ipv4_address, uint16_t port_num)
{
    serviceinfo s1;
    s1.Instance_ID = Instance_ID;
    s1.ipv4_address = ipv4_address;
    s1.port_num = port_num;
    return s1;
}
