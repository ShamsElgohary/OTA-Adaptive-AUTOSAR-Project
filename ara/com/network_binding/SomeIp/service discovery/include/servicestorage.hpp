#pragma once
#include <stdio.h>
#include <cstdint>

#include <vector>
#include <memory>
#include <map>

struct serviceinfo{
       uint16_t Instance_ID;
       uint32_t ipv4_address;
       uint8_t port_num;
};

class servicestorage{
/*
private:
//map <service id, service instance , port_num, ipaddress>;
//typedef std::map<int, std::pair<float, char> > Maptype;

struct serviceinfo {
       uint16_t Instance_ID;
       uint32_t ipv4_address;
       uint8_t port_num;
};

static std::map<uint16_t , serviceinfo> serviceRegistry;
//serviceRegistry::iterator itr;
//static serviceRegistry servicemap;
*/
public:

static std::map<uint16_t , serviceinfo> servicemap;
static void AddToServiceRegistry(uint16_t Service_ID,serviceinfo serviceinformation); //offer
static void RemoveService(uint16_t Service_ID);//stop offer
static void SearchServiceRegistry(uint16_t Service_ID,uint16_t Instance_ID);
static serviceinfo SetServiceInfo(uint16_t Instance_ID,uint32_t ipv4_address,uint8_t port_num);
};
