#pragma once
#include <iostream>
#include "../include/serviceProxy.hpp"
using namespace std;

namespace ara
{
    namespace com
    {
        namespace proxy
        {

            ProxyBase::ProxyBase(HandleType handle) : ProxyHandle(handle) {}
            ara::com::ServiceHandleContainer<ProxyBase::HandleType> ProxyBase::FindService(int serviceID)
            {
                ara::com::ServiceHandleContainer<ProxyBase::HandleType> vectorOfHandles;
                vector<ara::com::NetworkBindingBase::output> opVsomeip = ara::com::SomeIpNetworkBinding::FindService_SomeIp(serviceID);
                for (uint8_t i = 0; i < opVsomeip.size(); i++)
                {
                    ProxyBase::HandleType h;
                    h.ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(opVsomeip[i].instanceID, opVsomeip[i].port);
                    vectorOfHandles.push_back(h);
                }
                return vectorOfHandles;
            }
            ara::com::ServiceHandleContainer<ProxyBase::HandleType> ProxyBase::FindService(int serviceID, ara::com::InstanceIdentifier instanceId)
            {
                ara::com::ServiceHandleContainer<ProxyBase::HandleType> vectorOfHandles;
                if (1) //if someip
                {
                    vector<ara::com::NetworkBindingBase::output> opVsomeip = ara::com::SomeIpNetworkBinding::FindService_SomeIp(ProxyBase::serviceID);
                    for (uint8_t i = 0; i < opVsomeip.size(); i++)
                    {
                        ProxyBase::HandleType h;
                        h.ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(ProxyBase::serviceID, opVsomeip[i].instanceID, opVsomeip[i].port);
                        vectorOfHandles.push_back(h);
                    }
                    return vectorOfHandles;
                }
            }

        }
    }
}
