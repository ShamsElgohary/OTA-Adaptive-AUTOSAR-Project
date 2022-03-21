#include "../include/servicediscovery.hpp"
#include "../include/someip_sd_message.hpp"
//#include "SOMEIP_TCP.hpp"


using namespace std;
ServiceID serviceID=0xFFFF;
MethodID methodID=0x8100;
ClientID clientid=0x0000;
SessionID sessionID=0x0001;
ProtocolVersion protocol_version=0x01;
InterfaceVersion interface_version=0x01;
//ReturnCode returnCode=0x00;
someip::MessageType messagetype=someip::MessageType::NOTIFICATION;

servicediscovery::servicediscovery()
{

}

void servicediscovery::offer_service(uint16_t service_id,uint16_t instance_id,uint32_t ipv4_address, uint16_t port_num)
{  
   someip_sd_message message1;
   someip::someipHeader h1;
   std::stringstream ss;
   someip::someipMessage m1(h1,ss);
   message1.header=m1.header;
   service_entry *ptr=service_entry::create_offer_service_entry(service_id,instance_id); //return &service1
   service_entry s2=*ptr;
   std::unique_ptr<service_entry> ptr3 = unique_ptr<service_entry>(new service_entry(s2));
   message1.AddEntry(ptr3);
   ipv4_endpoint_option option1(ipv4_address,port_num);
   std::cout<<option1.getIPV4_Address();
    std::unique_ptr<ipv4_endpoint_option> ptr4 = unique_ptr<ipv4_endpoint_option>(new ipv4_endpoint_option(option1));
   message1.AddOption(ptr4);
   //message1
   SomeIpConfiguration someipConfig{TransportProtocol::TCP,EndUserType::CLIENT};
   boost::asio::io_service io_service;
   shared_ptr<someipConnection> clientUser = someipConnection::SetSomeIpConfiguration(io_service, 2067 , someipConfig);
   clientUser->SendMessage(message1);
   //someip::EndPoint e={"127.0. 0.1",80};
   //someip::SendsomeipMessage(message1,e);
   
}

void servicediscovery::find_service(uint16_t service_id,uint16_t instance_id)
{    
   someip_sd_message message1;
   someip::someipHeader h1;
   std::stringstream ss;
   someip::someipMessage m1(h1,ss);
   message1.header=m1.header;
   service_entry *ptr=service_entry::create_find_service_entry(service_id,instance_id); //return &service1
   service_entry s2=*ptr;
   std::unique_ptr<service_entry> ptr3 = unique_ptr<service_entry>(new service_entry(s2));
   message1.AddEntry(ptr3);
   //message1
   SomeIpConfiguration someipConfig{TransportProtocol::TCP,EndUserType::CLIENT};
   boost::asio::io_service io_service;
   shared_ptr<someipConnection> clientUser = someipConnection::SetSomeIpConfiguration(io_service, 2067 , someipConfig);
   clientUser->SendMessage(message1);
    uint32_t ttl;
    someip::someipMessage msg1;
    //msg1=someip::ReadsomeipMessage(boost::asio::ip::tcp::socket & socket);  //read

  
}

void servicediscovery::stop_offer_service(uint16_t service_id,uint16_t instance_id)
{
      someip_sd_message message1;
   someip::someipHeader h1;
   std::stringstream ss;
   someip::someipMessage m1(h1,ss);
   message1.header=m1.header;
   service_entry *ptr=service_entry::create_stop_offer_service_entry(service_id,instance_id); //return &service1
   service_entry s2=*ptr;
   std::unique_ptr<service_entry> ptr3 = unique_ptr<service_entry>(new service_entry(s2));
   message1.AddEntry(ptr3);
   //message1
   SomeIpConfiguration someipConfig{TransportProtocol::TCP,EndUserType::CLIENT};
   boost::asio::io_service io_service;
   shared_ptr<someipConnection> clientUser = someipConnection::SetSomeIpConfiguration(io_service, 2067 , someipConfig);
   clientUser->SendMessage(message1);

}

/*
int main()
{
instance=receive();
deserialize ();
parse 3shan type;
depending on type 
case() trigger map

//case search 
//construct msg
serialize msg
send (msg)
}

*/

/*

class servicestorage

private:
map <service id, service instance , port_num, ipaddress>

public:
static addmap() //offer
static remove() //stop offer
static return map info search() //find service
{
   
}


*/