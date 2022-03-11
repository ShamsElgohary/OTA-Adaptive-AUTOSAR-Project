#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include "../../core/types.hpp"

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
        
        class NetworkBase 
        {
            private:
                
                /* strInstanceId = "SOME/IP 21" */
                static BindingProtocol GetType(std::string instanceIdStr, uint16_t &instanceId);

            public:
                
                static std::shared_ptr<NetworkBase> Bind(uint16_t serviceId, std::string instanceId, uint16_t port);                

                /* STILL TO BE ADJUSTED */
                virtual void Send();
                virtual void Receive();
                virtual void SendRequest();
                virtual void OfferService();
                virtual void OfferService(std::string instanceId);
                virtual void StopOfferService(std::string instanceId);

        };
       
        
        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// SOMEIP NETWORK BINDING ///////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////

        class SomeIpNetworkBinding : public NetworkBase
        {
            private:
                
                uint16_t serviceId;
                uint16_t instanceId;
                uint16_t port;
            
            public:
                /* PORT BINDED TO THIS SPECIFIC SERVICE ID AND INSTANCE ID */
                SomeIpNetworkBinding(uint16_t serviceId, uint16_t instanceId, uint16_t port);
                void Send();
                void Receive();
                void SendRequest();
                void OfferService();
                void OfferService(std::string instanceId);
                void StopOfferService(std::string instanceId);

        };


        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// DDS NETWORK BINDING //////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////

        class DDSNetworkBinding : public NetworkBase
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

