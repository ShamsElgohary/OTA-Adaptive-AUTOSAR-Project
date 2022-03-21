
#include "servicediscovery.cpp"
#include "servicestorage.cpp"
#include "someip_sd_message.cpp"
#include "string.h"

uint16_t service_id;
uint16_t instance_id;
uint32_t TTL;
uint16_t TYPE;
uint32_t ipv4_address;
uint16_t port_num;

int main()
{
    while (1)
    {
        std::string flag;
        std::stringstream ss;
        uint32_t ttl;
        someip::someipMessage msg1;
       
        SomeIpConfiguration someipConfig{TransportProtocol::TCP, EndUserType::SERVER};

        boost::asio::io_service io_service;

        shared_ptr<someipConnection> serverUser = someipConnection::SetSomeIpConfiguration(io_service, 2067, someipConfig);

        serverUser->ServerListen();

        ss << serverUser->ReceiveMessage().payload;

        Deserializer D;
        D.Deserialize(ss, service_id, instance_id, TTL, TYPE);
        D.Deserialize(ss, ipv4_address, port_num);
        cout << "service_id"
             << " : " << service_id << endl;
        cout << "instance_id"
             << " : " << instance_id << endl;
        cout << "TTL"
             << " : " << TTL << endl;
        cout << "type"
             << " : " << TYPE << endl;
        cout << "address"
             << " : " << ipv4_address << endl;
        cout << "portnum"
             << " : " << port_num << endl;

        uint16_t SID = service_id;
        uint16_t IID = instance_id;
        uint32_t Address = ipv4_address;
        uint16_t port = port_num;

        switch (TYPE)

        {
        case (uint16_t)0x01:
        {
            if (TTL == (uint32_t)0xFFFFFF) // offer
            {
                cout << "in offer service" << endl;

                servicestorage::AddToServiceRegistry(SID, servicestorage::SetServiceInfo(IID, Address, port)); // return struct
                for (auto it = servicemap.cbegin(); it != servicemap.cend(); ++it)
                {
                    std::cout << it->first<<" ";
                    {
                        for (auto itr = it->second.begin(); itr != it->second.end(); ++itr)
                        {
                            cout << itr->Instance_ID << "  " << itr->ipv4_address << "  " << itr->port_num<<endl;
                        }
                    }
                }
            }
            else if (TTL == (uint32_t)0x000000) // stop offer  //only remove from map
            {
                cout << "in stop offer" << endl;

                servicestorage::RemoveService(SID, IID); // stop offer
                for (auto it = servicemap.cbegin(); it != servicemap.cend(); ++it)
                {
                    std::cout << it->first;
                    {
                        for (auto itr = it->second.begin(); itr != it->second.end(); ++itr)
                        {
                            cout << itr->Instance_ID << " " << itr->ipv4_address << " " << itr->port_num;
                        }
                    }
                }
            }
            break;
        }
        
        case (uint16_t)0x00: // find service
        {
            cout << "in find service" << endl;
            vector<serviceinfo> rtn;
            rtn = servicestorage::SearchServiceRegistry(SID, IID); // return typeee
        
             for (auto itr = rtn.begin(); itr != rtn.end(); ++itr)
                        {
                            cout << itr->Instance_ID << " " << itr->ipv4_address << " " << itr->port_num<<endl;
                        }
            break;
        } 
        default:
            cout << "yessss" << endl;
        }
    }
}
