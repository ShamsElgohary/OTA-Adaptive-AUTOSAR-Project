#pragma once
#include <stdio.h>
#include <cstdint>

#include <vector>
#include <memory>






class servicediscovery 
{
    public:
    servicediscovery();
    void offer_service(uint16_t service_id,uint16_t instance_id,uint32_t ipv4_address, uint16_t port_num);
    void find_service(uint16_t service_id,uint16_t instance_id);
    void stop_offer_service(uint16_t serviceID,uint16_t InstanceID);

};