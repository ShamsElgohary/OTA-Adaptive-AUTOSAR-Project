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
    addtoGUI(Service_ID, serviceinformation);
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
     Addtorequests(Service_ID,Instance_ID);
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




using namespace std;



void addtoGUI(uint16_t Service_ID, serviceinfo serviceinformation){
    Json::Value event;
    ifstream f("GUI_Report.json");
    Json::Reader R;
    R.parse(f, event);
    if (!event)
    {
        event["Cluster_name"] = "ServiceDiscovery";
        event["SD"]["ServiceInfoMap"] = Json::arrayValue;
        event["SD"]["Find Requests"] = Json::arrayValue;
         event["SD"]["Received SD messages"] = Json::arrayValue;
    }

    // Add To JSON FILE
    
    Json::Value Temp;
    Temp["Service ID"] = Service_ID;
    Temp["Instance ID"] = serviceinformation.Instance_ID;
    Temp["IP"] = serviceinformation.ipv4_address;
    Temp["Port Number"] = serviceinformation.port_num;

    event["SD"]["ServiceInfoMap"].append(Temp);

    //event["json"]["Arr2"].append(Temp);
    
    
    std::cout << event << std::endl;
    std::ofstream json_file("GUI_Report.json");
    json_file << event;
    json_file.close();

}

void Addtorequests(uint16_t Service_ID, uint16_t instance_id)
{
     Json::Value event;
    ifstream f("GUI_Report.json");
    Json::Reader R;
    R.parse(f, event);
    if (!event)
    {
        event["Cluster_name"] = "ServiceDiscovery";
        event["SD"]["ServiceInfoMap"] = Json::arrayValue;
        event["SD"]["Find Requests"] = Json::arrayValue;
        event["SD"]["Received SD messages"] = Json::arrayValue;
    }
    // Add To JSON FILE
    Json::Value Temp2;
    Temp2["Instance ID"] = instance_id;
    Temp2["Service ID"] = Service_ID;


    event["SD"]["Find Requests"].append(Temp2);
    
    
    std::cout << event << std::endl;
    std::ofstream json_file("GUI_Report.json");
    json_file << event;
    json_file.close();
}



void addmsgtoGUI(uint16_t Service_ID,uint16_t instance_id,uint32_t ttl,uint16_t TYPE,uint16_t port_num, string ipv4_address){
    Json::Value event;
    ifstream f("GUI_Report.json");
    Json::Reader R;
     string msgtype;
    R.parse(f, event);
    if (!event)
    {
        event["Cluster_name"] = "ServiceDiscovery";
        event["SD"]["ServiceInfoMap"] = Json::arrayValue;
        event["SD"]["Find Requests"] = Json::arrayValue;
        event["SD"]["Received SD messages"] = Json::arrayValue;
    }
    Json::Value Temp3;
    // Add To JSON FILE
    if (TYPE==0)
    {
         msgtype="find service";
         Temp3["Port Number"] = "_";
         ipv4_address="_";
    }
    else if((TYPE==1) && (ttl==0))
    {
        msgtype="Stop Offer service";
        Temp3["Port Number"] = "_";
        ipv4_address="_";
    }
    else {
        msgtype="offer service";
        Temp3["Port Number"] = port_num;}
        
    //Json::Value Temp3;
    Temp3["Service ID"] = Service_ID;
    Temp3["Instance ID"] = instance_id;
    Temp3["IP"] = ipv4_address;
    Temp3["Type"]=msgtype;
    Temp3["ttl"]=ttl;


    event["SD"]["Received SD messages"].append(Temp3);

    //event["json"]["Arr2"].append(Temp);
    
    cout<<"_________________________________________________________________"<<endl;
    std::cout << event << std::endl;
    std::ofstream json_file("GUI_Report.json");
    json_file << event;
    json_file.close();

}
