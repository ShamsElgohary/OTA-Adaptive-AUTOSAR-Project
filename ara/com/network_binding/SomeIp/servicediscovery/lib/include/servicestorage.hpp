#pragma once

#include <stdio.h>
#include <cstdint>

#include <vector>
#include <memory>
#include <map>
#include <stdlib.h>
#include <iostream>
#include "../../../Serialization.hpp"
#include "someip_sd_message.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <boost/filesystem.hpp>
#include "../../../../../../../utility/jsoncpp/header/json.h"
#include <string>
#include "../../../../../../../utility/simulation/include/simulation.hpp"
#include "../../../../../../../utility/general.hpp"

struct serviceinfo{
       uint16_t Instance_ID;
       std::string ipv4_address;
       uint16_t port_num;
};
static std::map<uint16_t , std::vector<serviceinfo>> servicemap;


class servicestorage{
private:
simulation sim_socket {8088};
public:

servicestorage();
void AddToServiceRegistry(uint16_t Service_ID,serviceinfo serviceinformation); //offer
void RemoveService(uint16_t Service_ID,uint16_t instance_id);//stop offer
std::vector<serviceinfo> SearchServiceRegistry(uint16_t Service_ID,uint16_t Instance_ID);
serviceinfo SetServiceInfo(uint16_t Instance_ID,std::string ipv4_address,uint16_t port_num);


void addtoGUI(uint16_t Service_ID, serviceinfo serviceinformation);
void Addtorequests(uint16_t Service_ID, uint16_t instance_id);
void addmsgtoGUI(uint16_t Service_ID,uint16_t instance_id,uint32_t ttl,uint16_t TYPE,uint16_t port_num, string ipv4_address,someip::MessageType m, MessageID messageID,ProtocolVersion protocol_version,InterfaceVersion interface_version);
void removefromlist();

};

