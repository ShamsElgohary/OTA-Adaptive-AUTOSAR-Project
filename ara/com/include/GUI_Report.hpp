#pragma once
#include <iostream>

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
        void AddMethodCall(int methodID, string methodName, MethodType Type);
        void AddServiceDiscoveryRequest(uint16_t ServiceID, uint16_t InstanceID, ServiceDiscoveryMethodType Type, bool GrantResult);
    }
}
