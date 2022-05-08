#include "../include/servicediscovery.hpp"

using namespace std;
using namespace someip;
ServiceID serviceID = 0xFFFF;
MethodID methodID = 0x8100;
// ClientID clientid = 0x0000;
// SessionID sessionID = 0x0001;
ProtocolVersion protocol_version = 0x01;
InterfaceVersion interface_version = 0x01;
// ReturnCode returnCode=0x00;
someip::MessageType messagetype = someip::MessageType::NOTIFICATION;

servicediscovery::servicediscovery()
{}

void servicediscovery::offer_service(uint16_t service_id, uint16_t instance_id, std::string ipv4_address, uint16_t port_num)
{
    someip_sd_message message1;
    someip::someipHeader h1;
    std::stringstream ss;
    someip::someipMessage m1(h1, ss);
    message1.header = m1.header;
    service_entry *ptr = service_entry::create_offer_service_entry(service_id, instance_id); // return &service1
    service_entry s2 = *ptr;
    std::unique_ptr<service_entry> ptr3 = unique_ptr<service_entry>(new service_entry(s2));
    message1.AddEntry(ptr3);
    ipv4_endpoint_option option1(ipv4_address, port_num);
    std::unique_ptr<ipv4_endpoint_option> ptr4 = unique_ptr<ipv4_endpoint_option>(new ipv4_endpoint_option(option1));
    message1.AddOption(ptr4);
    // message1
    SomeIpConfiguration someipConfig{TransportProtocol::TCP, EndUserType::CLIENT, SecurityType::RAW};
    boost::asio::io_context  io_service;
    shared_ptr<someipConnection> clientUser = someipConnection::SetSomeIpConfiguration(io_service, 2067, someipConfig);
    clientUser->SendMessage(message1);
}

std::vector<serviceinfo> servicediscovery::find_service(uint16_t service_id, uint16_t instance_id)
{
    someip_sd_message message1;
    someip::someipHeader h1;
    std::stringstream ss;
    someip::someipMessage m1(h1, ss);
    message1.header = m1.header;
    service_entry *ptr = service_entry::create_find_service_entry(service_id, instance_id); // return &service1
    service_entry s2 = *ptr;
    std::unique_ptr<service_entry> ptr3 = unique_ptr<service_entry>(new service_entry(s2));
    message1.AddEntry(ptr3);
    SomeIpConfiguration someipConfig{TransportProtocol::TCP, EndUserType::CLIENT, SecurityType::RAW};
    boost::asio::io_context  io_service;
    shared_ptr<someipConnection> clientUser = someipConnection::SetSomeIpConfiguration(io_service, 2067, someipConfig);
    clientUser->SendMessage(message1);
    ss << clientUser->ReceiveMessage().payload;
    vector<uint16_t> v1;
    vector<std::string> v2;
    vector<std::uint16_t> v3;
    Deserializer d;
    d.Deserialize(ss, v1);
    d.Deserialize(ss, v2);
    d.Deserialize(ss, v3);

    std::vector<serviceinfo> rtn;
      vector<std::string> ::iterator it2=v2.begin();
    vector<std::uint16_t> ::iterator it3=v3.begin();
    for (auto itr = v1.begin(); itr != v1.end(); ++itr)
    {    
    
        serviceinfo str;
        str.Instance_ID = *itr;
        //v1.pop_back();
        str.ipv4_address = *it2;
        //v2.pop_back();
        str.port_num = *it3;
        //v3.pop_back();
        rtn.push_back(str);
       it2++;
       it3++;
    }
   
    return rtn;
}

void servicediscovery::stop_offer_service(uint16_t service_id, uint16_t instance_id)
{
    someip_sd_message message1;
    someip::someipHeader h1;
    std::stringstream ss;
    someip::someipMessage m1(h1, ss);
    message1.header = m1.header;
    service_entry *ptr = service_entry::create_stop_offer_service_entry(service_id, instance_id); // return &service1
    service_entry s2 = *ptr;
    std::unique_ptr<service_entry> ptr3 = unique_ptr<service_entry>(new service_entry(s2));
    message1.AddEntry(ptr3);
    // message1
    SomeIpConfiguration someipConfig{TransportProtocol::TCP, EndUserType::CLIENT, SecurityType::RAW};
    boost::asio::io_context  io_service;
    shared_ptr<someipConnection> clientUser = someipConnection::SetSomeIpConfiguration(io_service, 2067, someipConfig);
    clientUser->SendMessage(message1);
}