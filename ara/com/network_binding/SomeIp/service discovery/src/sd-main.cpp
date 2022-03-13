#include "servicediscovery.hpp"
#include"servicestorage.hpp"
#include "../include/someip_sd_message.hpp"
#include "../../SOMEIP.hpp"



int main()
{    
   while (1)
   {
    uint16_t ttl;
    someip::someip_Message msg1;
    //msg1=someip::ReadsomeipMessage(boost::asio::ip::tcp::socket & socket);  //read
    Deserializer D;
    std::stringstream ss;
    D.Deserialize(ss,msg1.payload); //msg1.payload
    someip_sd_message s1;
    //someip::someipHeader h1();
    s1.payload=msg1.payload;
    std::unique_ptr <entry> ptr1=s1.GetEntry();
    std::unique_ptr <ipv4_endpoint_option> ptr2=s1.GetOption();
    ttl=ptr1->getTimeToLIve();
    switch(ptr1->getType()){
        case 0x01:
        {
            if (ttl=0xFFFFFF)  //offer
            { 
                uint16_t SID=ptr1->getServiceID();
                uint16_t IID=ptr1->getInstanceID();
                uint32_t Address=ptr2->getIPV4_Address();
                uint8_t port= ptr2->getport_num();
                servicestorage s1;
                s1.AddToServiceRegistry(SID,s1.SetServiceInfo(IID,Address,port));  //return struct 
            }
            else if (ttl=0x000000) //stop offer  //only remove from map
            {
                servicestorage s2;
                uint16_t SID=ptr1->getServiceID();
                s2.RemoveService(SID);//stop offer
            }
        break;
        }
        case 0x00:  //find service
        {
            servicestorage s1;
            uint16_t SID=ptr1->getServiceID();
            uint16_t IID=ptr1->getInstanceID();
            s1.SearchServiceRegistry(SID,IID);  //return typeee 
            //serialize
            //send 
            break;
        }
        }

   }



}