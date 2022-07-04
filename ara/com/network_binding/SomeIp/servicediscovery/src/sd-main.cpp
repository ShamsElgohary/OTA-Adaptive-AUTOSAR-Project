
#include "../lib/include/servicediscovery.hpp"
#include "../lib/include/servicestorage.hpp"
#include "../lib/include/someip_sd_message.hpp"
#include "string.h"
#include "../../../../../exec/include/execution_client.hpp"
using namespace ara::exec;

uint16_t service_id;
uint16_t instance_id;
uint32_t TTL;
uint16_t TYPE;
uint16_t port_num;
void handle_sigTerm(int sig)
{
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::Kterminate);
    exit(1);
}
int main()
{
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handle_sigTerm;
    sigaction(SIGTERM, &sa, NULL);
    
    ara::exec::ExecutionClient exec;
    exec.ReportExecutionStaste(ara::exec::ExecutionState::Krunning);

    sleep(1);
    
    servicestorage ServiceStorage;
    Json::Value event;
    ifstream f("SD_Report.json");
    Json::Reader R;
    R.parse(f, event);
    event["Cluster_name"] = "ServiceDiscovery";
    event["SD"]["ServiceInfoMap"] = Json::arrayValue;
    event["SD"]["Find Requests"] = Json::arrayValue;
    event["SD"]["Received SD messages"] = Json::arrayValue;
    std::ofstream json_file("SD_Report.json");
    json_file << event;
    json_file.close();


    while (1)
    {
        std::string ipv4_address;
        std::string flag;
        std::stringstream ss;
        uint32_t ttl;
        someip::someipMessage msg1;

        someip::SomeIpConfiguration someipConfig{someip::TransportProtocol::TCP, someip::EndUserType::SERVER, someip::SecurityType::RAW};

        boost::asio::io_context io_service;

        std::shared_ptr<someip::someipConnection> serverUser = someip::someipConnection::SetSomeIpConfiguration(io_service, 2067, someipConfig);

        std::cout << "[servicediscovery] "
                  << "Listening For SD Requests..." << std::endl
                  << std::endl;

        serverUser->ServerListen();

        ss << serverUser->ReceiveMessage().payload;

        Deserializer D;
        D.Deserialize(ss, service_id, instance_id, TTL, TYPE);
        D.Deserialize(ss, ipv4_address);
        D.Deserialize(ss, port_num);
        

        ServiceStorage.addmsgtoGUI(service_id,instance_id,TTL,TYPE,port_num,ipv4_address);

        uint16_t SID = service_id;
        uint16_t IID = instance_id;
        std::string Address = ipv4_address;
        uint16_t port = port_num;

        switch (TYPE)

        {
        case (uint16_t)0x01:
        {
            if (TTL == (uint32_t)0xFFFFFF) // offer
            {
                ServiceStorage.AddToServiceRegistry(SID, ServiceStorage.SetServiceInfo(IID, Address, port)); // return struct
            }
            else if (TTL == (uint32_t)0x000000) // stop offer  //only remove from map
            {
                ServiceStorage.RemoveService(SID, IID); // stop offer
            }
            break;
        }

        case (uint16_t)0x00: // find service
        {

            std::vector<serviceinfo> rtn;
            rtn = ServiceStorage.SearchServiceRegistry(SID, IID); // return typeee

            std::vector<uint16_t> instance_ids;
            std::vector<std::string> addresses;
            std::vector<uint16_t> port_num;
            for (auto itr = rtn.begin(); itr != rtn.end(); ++itr)
            {

                cout << "[servicediscovery] " << itr->Instance_ID << " " << itr->ipv4_address << " " << itr->port_num << endl;
                instance_ids.push_back(itr->Instance_ID);
                addresses.push_back(itr->ipv4_address);
                port_num.push_back(itr->port_num);
            }

            someip_sd_message msg1;
            someip::someipHeader header1;
            std::stringstream ss, ss1, ss2;
            someip::someipMessage someipmsg(header1, ss);
            msg1.header = someipmsg.header;
            Serializer s;
            s.Serialize(ss, instance_ids);
            msg1.payload += ss.str();
            s.Serialize(ss1, addresses);
            msg1.payload += ss1.str();
            s.Serialize(ss2, port_num);
            msg1.payload += ss2.str();
            serverUser->SendMessage(msg1);
            break;
        }
        default:
            std::cout << "[servicediscovery] "
                      << "default" << std::endl;
        }
    }
}
