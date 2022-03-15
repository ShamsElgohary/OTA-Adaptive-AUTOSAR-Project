
#include "servicediscovery.cpp"
#include"servicestorage.hpp"
#include "someip_sd_message.cpp"
#include "string.h"

uint16_t service_id;
uint16_t instance_id;
uint16_t TTL;
uint8_t type;
uint32_t ipv4_address;
uint8_t port_num;

int main()
{    
   while (1)
   {
    std::string flag;
    std::stringstream ss;
    uint16_t ttl;
    someip::someip_Message msg1;
    //msg1=someip::ReadsomeipMessage(boost::asio::ip::tcp::socket & socket);  //read

    SomeIpConfiguration someipConfig{TransportProtocol::TCP,EndUserType::SERVER};

    boost::asio::io_service io_service;

    shared_ptr<someipEndUser> serverUser = someipEndUser::SetSomeIpConfiguration(io_service, MULTICAST_PORT , someipConfig);

    serverUser->ServerListen();

    ss << serverUser->ReceiveMessage().payload;

    cout << ss.str()<<endl;
    
    /*

    ss<<msg1.payload;
    Deserializer D;
    D.Deserialize(ss,flag);
    if (flag.compare("entry"))
    {
        D.Deserialize(ss,service_id,instance_id,TTL,type);
    }
      D.Deserialize(ss,flag); 
    if (flag.compare("option"))
    {
        D.Deserialize(ss,ipv4_address,port_num);
    }

    service_entry entry1(service_id,instance_id);
    someip_sd_message s1;
    //someip::someipHeader h1();
    //s1.payload=msg1.payload;
    //std::unique_ptr <entry> ptr1=s1.GetEntry();
    //std::unique_ptr <ipv4_endpoint_option> ptr2=s1.GetOption();
   // ttl=ptr1->getTimeToLIve();
    switch(type){
        case 0x01:
        {
            if (ttl=(uint16_t)0xFFFFFF)  //offer
            { 
                uint16_t SID=service_id;
                uint16_t IID=instance_id;
                uint32_t Address=ipv4_address;
                uint8_t port=port_num;
                servicestorage s1;
                s1.AddToServiceRegistry(SID,s1.SetServiceInfo(IID,Address,port));  //return struct 
            }
            else if (ttl=0x000000) //stop offer  //only remove from map
            {
                servicestorage s2;
                uint16_t SID=service_id;
                s2.RemoveService(SID);//stop offer
            }
        break;
        }
        case 0x00:  //find service
        {
            servicestorage s1;
            uint16_t SID=service_id;
            uint16_t IID=instance_id;
            s1.SearchServiceRegistry(SID,IID);  //return typeee 
            
            //send 
            break;
        }
        }

   }
   */

}}

