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
            class ProxyBase
            {
                ProxyBase(HandleType handle)
                {
                    ara::com::proxy::ProxyBase::ProxyHandle = handle;
                }

                static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService()
                {
                    ara::com::ServiceHandleContainer<ara::com::proxy::HandleType> vectorOfHandles;
                    vector<ara::com::proxy::SOMEIP::output> opVsomeip = ara::com::proxy::SOMEIP::FindService(instanceId);
                    vector<ara::com::proxy::DDS::output> opVdds = ara::com::proxy::DDS::FindService(instanceId);

                    //handle construction
                    for (uint8_t i = 0; i < opVsomeip.size(); i++)
                    {
                        HandleType h;
                        //h.bindingProtocol = BindingProtocol::someip;
                        h.networkBindingPtr = new ara::com::SomeIpNetworkBinding;
                        h.InstanceID = opVsomeip[i].instanceID;
                        h.ip = opVsomeip[i].ip;
                        h.port = opVsomeip[i].port;
                        h.serviceID = ara::com::proxy::ProxyBase::serviceID; //will this be hardcoded?
                        vectorOfHandles.push_back(h);
                    }

                    for (uint8_t i = 0; i < opVdds.size(); i++)
                    {
                        HandleType h;
                        //h.bindingProtocol = BindingProtocol::dds;
                        h.networkBindingPtr = new ara::com::DDSNetworkBinding;
                        h.InstanceID = opVdds[i].instanceID;
                        h.ip = opVdds[i].ip;
                        h.port = opVdds[i].port;
                        h.serviceID = ara::com::proxy::ProxyBase::serviceID;
                        vectorOfHandles.push_back(h);
                    }
                }

                static ara::com::ServiceHandleContainer<ara::com::proxy::HandleType> FindService(ara::com::InstanceIdentifier instanceId)
                {
                    ara::com::ServiceHandleContainer<ara::com::proxy::HandleType> vectorOfHandles;
                    //parse manifest using instanceId to get bindingType of service to call the suitable findservice

                    if (type == BindingProtocol::someip)
                    {
                        vector<ara::com::proxy::SOMEIP::output> opV = ara::com::proxy::SOMEIP::FindService(instanceId);
                        //handle construction
                        for (uint8_t i = 0; i < opV.size(); i++)
                        {
                            HandleType h;
                            //h.bindingProtocol = BindingProtocol::someip;
                            h.networkBindingPtr = new ara::com::SomeIpNetworkBinding;
                            h.InstanceID = instanceId;
                            h.ip = opV[i].ip;
                            h.port = opV[i].port;
                            h.serviceID = ara::com::proxy::ProxyBase::serviceID;
                            vectorOfHandles.push_back(h);
                        }
                    }
                    else
                    {
                    }
                    return vectorOfHandles;
                }

                ~ProxyBase() { delete ara::com::proxy::ProxyBase::ProxyHandle.ara::com::proxy::HandleType::networkBindingPtr; }
            };
        }
    }
}
