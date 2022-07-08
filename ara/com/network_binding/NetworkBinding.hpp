#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include "../include/types.hpp"
#include "SomeIp/servicediscovery/lib/include/servicediscovery.hpp"
#include "SomeIp/servicediscovery/lib/include/servicestorage.hpp"

using namespace std;

namespace ara
{
    namespace com
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////  NETWORKBASE CLASS INTERFACE CLASS ////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        class NetworkBindingBase
        {
        public:
            NetworkBindingBase() = default;
            ~NetworkBindingBase() = default;
            
            virtual void OfferService() = 0;
            virtual void StopOfferService() = 0;

            virtual stringstream ReceiveMessage(int &method_id) = 0;
            virtual stringstream ReceiveMessage() = 0;

            virtual stringstream SendRequest(uint32_t methodID, stringstream &s) = 0;            
            virtual void SendResponse(uint32_t methodID, stringstream &s) = 0;
            virtual void SendFireAndForget(uint32_t methodID, stringstream &s) = 0;
            virtual void SendNotification(uint32_t methodID, stringstream &s) = 0;
            virtual void ServerListen() = 0;
            
            virtual void CloseConnection() = 0;
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// SOMEIP NETWORK BINDING ///////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        class SomeIpNetworkBinding : public NetworkBindingBase
        {

        private:
            ServiceID serviceId;
            InstanceID InstanceId;
            uint16_t port;
            std::string ip;
            boost::asio::io_context io_service;
            someip::SomeIpConfiguration someipConfig;
            std::shared_ptr<someip::someipConnection> clientInstance = nullptr;

        public:
            SomeIpNetworkBinding(string ip, uint16_t port);
            SomeIpNetworkBinding(serviceIdentifierType service_id, InstanceIdentifier instance_id,
                                                   string ip, uint16_t port, someip::EndUserType type);

            void OfferService() override;
            void StopOfferService() override;
            void ServerListen() override;

            stringstream SendRequest(uint32_t methodID, stringstream &s) override;            
            void SendResponse(uint32_t methodID, stringstream &s) override;
            void SendFireAndForget(uint32_t methodID, stringstream &s) override;
            void SendNotification(uint32_t methodID, stringstream &s) override;

            void CloseConnection() override;
            
            stringstream ReceiveMessage(int &method_id) override;
            stringstream ReceiveMessage() override;
            
            static vector<serviceinfo> FindService_SomeIp(int serviceID, ara::com::InstanceIdentifier instance_id = 0xffff);

        };
    }
}
