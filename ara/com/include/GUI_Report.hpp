#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <boost/filesystem.hpp>
#include "../../../utility/jsoncpp/header/json.h"
#include "../../../utility/simulation/include/simulation.hpp"
#include "../../../utility/general.hpp"

using namespace std;

namespace ara
{
    namespace com
    {
        enum class MethodType : uint8_t
        {
            Proxy_Method = 1U,
            Skeleton_Method = 2U
        };
        enum class ServiceDiscoveryMethodType : uint8_t
        {
            Offer_Method = 1U,
            Find_Method = 2U,
            Stop_Method = 3U
        };
        
        void AddMethodCall(int methodID, string methodName, MethodType Type, uint16_t ServiceID, string Cluster_Name);
        void AddServiceDiscoveryRequest(uint16_t ServiceID, uint16_t InstanceID, ServiceDiscoveryMethodType Type, bool GrantResult, string Cluster_Name);
    }
}
