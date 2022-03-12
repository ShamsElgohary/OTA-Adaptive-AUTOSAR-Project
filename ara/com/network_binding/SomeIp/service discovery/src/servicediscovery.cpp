#include "../include/servicediscovery.hpp"
#include "../include/entry.hpp"
#include "../include/option.hpp"
#include "../../SOMEIP.hpp"
#include "../../Serialization.hpp"
#include "../include/someip_sd_message.hpp"

ServiceID serviceID=0xFFFF;
MethodID methodID=0x8100;
ClientID clientid=0x0000;
SessionID sessionID=0x0001;
ProtocolVersion protocol_version=0x01;
InterfaceVersion interface_version=0x01;
ReturnCode returnCode=0x00;
MessageType messagetype=MessageType::NOTIFICATION;

void servicediscovery::offer_service(uint16_t service_id,uint16_t instance_id,uint32_t ipv4_address,uint32_t port_num)
{  
   someip_sd_message message1;
   someip::someipHeader h1(serviceID,methodID,clientid,sessionID,protocol_version,interface_version,messagetype,returnCode);
   message1.header=h1;
   service_entry s1=service_entry::create_offer_service_entry(service_id,instance_id);
   message1.AddEntry(std::make_unique<service_entry>(s1));
   ipv4_endpoint_option option1(ipv4_address,port_num);
   message1.AddOption(std::make_unique<ipv4_endpoint_option>(option1));
   someip::EndPoint ep;
   someip::SendsomeipMessage(message1,ep);
}

void servicediscovery::find_service(uint16_t service_id,uint16_t instance_id)
{    
     someip_sd_message message1;
     someip::someipHeader h1(serviceID,methodID,clientid,sessionID,protocol_version,interface_version,messagetype,returnCode);
      message1.header=h1;
     someip_sd_message message1;
     service_entry s1=service_entry::create_find_service_entry(service_id,instance_id);
     message1.AddEntry(std::make_unique<service_entry>(s1));
     someip::EndPoint ep;
     someip::SendsomeipMessage(message1,ep);


}

void servicediscovery::stop_offer_service(uint16_t service_id,uint16_t instance_id)
{
     someip_sd_message message1;
     someip::someipHeader h1(serviceID,methodID,clientid,sessionID,protocol_version,interface_version,messagetype,returnCode);
     message1.header=h1;
     service_entry s1=service_entry::create_stop_offer_service_entry(service_id,instance_id);
     message1.AddEntry(std::make_unique<service_entry>(s1));
     someip::EndPoint ep;
     someip::SendsomeipMessage(message1,ep);

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