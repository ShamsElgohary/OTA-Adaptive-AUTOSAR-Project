#pragma once
#include <stdio.h>
#include <cstdint>

#include <vector>
#include <memory>
#include"servicestorage.hpp"
#include "someip_sd_message.hpp"
#include "../../../someip.hpp"






class servicediscovery 
{
    public:
    servicediscovery();
    static void offer_service(uint16_t service_id,uint16_t instance_id,std::string ipv4_address, uint16_t port_num);
    static std::vector<serviceinfo> find_service(uint16_t service_id,uint16_t instance_id);
    static void stop_offer_service(uint16_t serviceID,uint16_t InstanceID);

};