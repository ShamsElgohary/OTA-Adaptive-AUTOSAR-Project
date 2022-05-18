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
    Addtorequests(Service_ID, Instance_ID);
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

simulation sim(8088);
Json::Value Temp2;
Json::Value Temp3;
void removefromlist()

{
    Json::Value event;
    ifstream f("GUI_Report.json");

    Json::Reader R;
    R.parse(f, event);
    if (!event)
    {
        string x = " ";
        event["Cluster_name"] = "ServiceDiscovery";
        event["SD"]["ServiceInfoMap"] = Json::arrayValue;
        event["SD"]["Find Requests"] = Json::arrayValue;
        event["SD"]["Received SD messages"] = Json::arrayValue;
    }

    // Add To JSON FILE
    event["SD"]["ServiceInfoMap"].clear();
    Json::Value Temp4;
    auto itr = servicemap.begin();
    for (auto itr = servicemap.begin(); itr != servicemap.end(); ++itr)
    {
        Temp4["Service ID"] = itr->first;
        for (auto it = itr->second.begin(); it != itr->second.end(); ++it)
        {
            Temp4["Instance ID"] = it->Instance_ID;
            Temp4["IP"] = it->ipv4_address;
            Temp4["Port Number"] = it->port_num;
            event["SD"]["ServiceInfoMap"].append(Temp4);
        }
    }

    std::cout << event << std::endl;
    std::ofstream json_file("GUI_Report.json");
    json_file << event;
    json_file.close();

    if (SIMULATION_ACTIVE)
    {
        sim.connect_to_socket();

        char current_dir[256];

        getcwd(current_dir, 256);

        std::string path(current_dir);

        path += "/GUI_Report.json";

        sim.send_file((char *)(path.c_str()));
    }
}

void addtoGUI(uint16_t Service_ID, serviceinfo serviceinformation)
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

    Json::Value Temp;
    Temp["Service ID"] = Service_ID;
    Temp["Instance ID"] = serviceinformation.Instance_ID;
    Temp["IP"] = serviceinformation.ipv4_address;
    Temp["Port Number"] = serviceinformation.port_num;

    event["SD"]["ServiceInfoMap"].append(Temp);

    std::cout << event << std::endl;
    std::ofstream json_file("GUI_Report.json");
    json_file << event;
    json_file.close();

    if (SIMULATION_ACTIVE)
    {
        sim.connect_to_socket();

        char current_dir[256];

        getcwd(current_dir, 256);

        std::string path(current_dir);

        path += "/GUI_Report.json";

        sim.send_file((char *)(path.c_str()));
    }
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

    Temp2["Instance ID"] = instance_id;
    Temp2["Service ID"] = Service_ID;

    event["SD"]["Find Requests"].append(Temp2);

    std::cout << event << std::endl;
    std::ofstream json_file("GUI_Report.json");
    json_file << event;
    json_file.close();

    /*connect to server*/
    if (SIMULATION_ACTIVE)
    {
        sim.connect_to_socket();
        char current_dir[256];
        getcwd(current_dir, 256);
        std::string path(current_dir);
        path += "/GUI_Report.json";
        sim.send_file((char *)(path.c_str()));
    }
}

void addmsgtoGUI(uint16_t Service_ID, uint16_t instance_id, uint32_t ttl, uint16_t TYPE, uint16_t port_num, string ipv4_address)
{

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
    if (TYPE == 0)
    {
        msgtype = "find service";
        Temp3["Port Number"] = "_";
        ipv4_address = "_";
    }
    else if ((TYPE == 1) && (ttl == 0))
    {
        msgtype = "Stop Offer service";
        Temp3["Port Number"] = "_";
        ipv4_address = "_";
    }
    else
    {
        msgtype = "offer service";
        Temp3["Port Number"] = port_num;
    }

    Temp3["Service ID"] = Service_ID;
    Temp3["Instance ID"] = instance_id;
    Temp3["IP"] = ipv4_address;
    Temp3["Type"] = msgtype;
    Temp3["ttl"] = ttl;

    event["SD"]["Received SD messages"].append(Temp3);

    std::cout << event << std::endl;
    std::ofstream json_file("GUI_Report.json");
    json_file << event;
    json_file.close();
    if (SIMULATION_ACTIVE)
    {
        sim.connect_to_socket();
        char current_dir[256];
        getcwd(current_dir, 256);
        std::string path(current_dir);
        path += "/GUI_Report.json";
        sim.send_file((char *)(path.c_str()));
    }

    /*connect to server*/
}
