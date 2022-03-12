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



void servicestorage::SearchServiceRegistry(uint16_t Service_ID,uint16_t Instance_ID)
{
    if (Instance_ID !=0xFFFF){
    std::map<uint16_t, serviceinfo>::iterator itr;
    /* IF THE CORRECT ID IS FOUND RETURN THE PACKAGE CORRESPONDING TO THIS ID*/
    for (itr = servicemap.begin(); itr != servicemap.end(); ++itr)
    {
        if (itr->first == Service_ID)
        {
            //return (itr->second);
            if ((itr->second).Instance_ID==Instance_ID)
            {
              std::cout <<"service found";
            }
        }
    }}
// id (instance_id)=0xfff

}

serviceinfo servicestorage::SetServiceInfo(uint16_t Instance_ID,uint32_t ipv4_address,uint8_t port_num){   //returnstruct
    serviceinfo s1;
    s1.Instance_ID=Instance_ID;
    s1.ipv4_address=ipv4_address;
    s1.port_num=port_num;
    return s1;
} 
