#include <stdio.h>
#include <cstdint>

#include <vector>
#include <memory>
#include <map>
struct searchreturn{
       uint16_t ServiceID;
       uint16_t Instance_ID;
       uint32_t ipv4_address;
       uint8_t port_num;
};

struct serviceinfo{
       uint16_t Instance_ID;
       uint32_t ipv4_address;
       uint8_t port_num;
};

class servicestorage{

public:
static std::map<uint16_t , serviceinfo> servicemap;
static void AddToServiceRegistry(uint16_t Service_ID,serviceinfo serviceinformation); //offer
static void RemoveService(uint16_t Service_ID);//stop offer
static searchreturn SearchServiceRegistry(uint16_t Service_ID,uint16_t Instance_ID);
static serviceinfo SetServiceInfo(uint16_t Instance_ID,uint32_t ipv4_address,uint8_t port_num);
};
