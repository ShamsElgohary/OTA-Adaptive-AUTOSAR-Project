#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include "../../core/types.hpp"
#include "../../com/network_binding/SomeIp/service_discovery/include/servicediscovery.hpp"

using namespace std;

namespace ara
{
    namespace com
    {

        enum BindingProtocol
        {
            SOMEIP,
            DDS
        };
        
        /////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////  NETWORKBASE CLASS INTERFACE CLASS ////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        
        class NetworkBindingBase 
        {
            private:
                
                /* strInstanceId = "SOME/IP 21" */
                static BindingProtocol GetType(std::string instanceIdStr, uint16_t &instanceId);

            public:
                
                static std::shared_ptr<NetworkBindingBase> Bind(uint16_t serviceId, std::string instanceId, uint16_t port);                

                /* STILL TO BE ADJUSTED */
                virtual void Send();
                virtual void Receive();
                template<typename T,typename...Params>
                void SendRequest(uint32_t methodID,T a , Params...args);
                virtual void OfferService();
                //virtual void OfferService(std::string instanceId);
                virtual void StopOfferService();

        };
       
        
        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// SOMEIP NETWORK BINDING ///////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////

        class SomeIpNetworkBinding : public NetworkBindingBase
        {
            private:
                
                uint16_t serviceId;
                uint16_t instanceId;
                uint16_t port;
                uint32_t ipv4_address;
            
            public:
                /* PORT BINDED TO THIS SPECIFIC SERVICE ID AND INSTANCE ID */
                SomeIpNetworkBinding(uint16_t serviceId, uint16_t instanceId, uint16_t port,uint32_t ipv4_address);
                void Send();
                void Receive();
                void SendRequest();
                void OfferService();
                //void OfferService(std::string instanceId);
                void StopOfferService();

        };


        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// DDS NETWORK BINDING //////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////

        class DDSNetworkBinding : public NetworkBindingBase
        {
            private:
                
                uint16_t serviceId;
                std::string instanceIdStr;
                uint16_t port;
            
            public:
                /* PORT BINDED TO THIS SPECIFIC SERVICE ID AND INSTANCE ID */
                DDSNetworkBinding(uint16_t serviceId, std::string instanceIdStr, uint16_t port);
                void Send();
                void Receive();
                void SendRequest();
                void OfferService();
                void OfferService(std::string instanceId);
                void StopOfferService(std::string instanceId);

        };

    }
}

