#pragma once

#include <stdio.h>
#include <cstdint>

#include <vector>
#include <memory>
#include <map>



struct serviceinfo{
       uint16_t Instance_ID;
       uint32_t ipv4_address;
       uint16_t port_num;
};
static std::map<uint16_t , std::vector<serviceinfo>> servicemap;

class servicestorage{

public:
static void AddToServiceRegistry(uint16_t Service_ID,serviceinfo serviceinformation); //offer
static void RemoveService(uint16_t Service_ID,uint16_t instance_id);//stop offer
static std::vector<serviceinfo> SearchServiceRegistry(uint16_t Service_ID,uint16_t Instance_ID);
static serviceinfo SetServiceInfo(uint16_t Instance_ID,uint32_t ipv4_address,uint8_t port_num);
};
