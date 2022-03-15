#include "../include/servicestorage.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


void servicestorage::AddToServiceRegistry(uint16_t Service_ID,serviceinfo serviceinformation)
{
    servicemap.insert(std::make_pair(Service_ID,serviceinformation));
    //emplace
}

 void servicestorage::RemoveService(uint16_t Service_ID)
{
    servicemap.erase(Service_ID);
}




searchreturn servicestorage::SearchServiceRegistry(uint16_t Service_ID,uint16_t Instance_ID)
{
    std::map<uint16_t, serviceinfo>::iterator itr;
    if (Instance_ID !=0xFFFF){
    /* IF THE CORRECT ID IS FOUND RETURN THE PACKAGE CORRESPONDING TO THIS ID*/
    for (itr = servicemap.begin(); itr != servicemap.end(); ++itr)
    {
        if (itr->first == Service_ID)
        {
            //return (itr->second);
            if ((itr->second).Instance_ID==Instance_ID)
            {
              std::cout <<"service found";
              searchreturn s1;
              s1.ServiceID=Service_ID;
              s1.Instance_ID=Instance_ID;
              s1.ipv4_address=itr->second.ipv4_address;
              s1.port_num=itr->second.port_num;
              return s1;
            }
        }
    }}
    else {
         for (itr = servicemap.begin(); itr != servicemap.end(); ++itr)
    {
        if (itr->first == Service_ID)
        { 
            std::cout<<"service found";
              searchreturn s1;
              s1.ServiceID=Service_ID;
              s1.Instance_ID=(uint16_t)0xFFFF;
              s1.ipv4_address=itr->second.ipv4_address;
              s1.port_num=itr->second.port_num;
              return s1;
        
        }
    }
    }
              searchreturn s1;
              s1.ServiceID=Service_ID;
              s1.Instance_ID=(uint16_t)0xFFFF;
              s1.ipv4_address=(uint32_t)0xFFFF;
              s1.port_num=(uint8_t)0xFFFF;
              return s1;
    
    

}

serviceinfo servicestorage::SetServiceInfo(uint16_t Instance_ID,uint32_t ipv4_address,uint8_t port_num){   //returnstruct
    serviceinfo s1;
    s1.Instance_ID=Instance_ID;
    s1.ipv4_address=ipv4_address;
    s1.port_num=port_num;
    return s1;
} 
