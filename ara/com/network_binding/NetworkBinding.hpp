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
            static std::shared_ptr<NetworkBindingBase> Bind(int serviceId, int instanceId);

            template <typename... Params>
            void SendRequest(uint32_t methodID, Params... args);
            template <typename T>
            void ReceiveMessage(T &in);

            virtual void OfferService();
            void StopOfferService();
            int get_method_id();
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

        boost::asio::io_service io_service;
        // TEST
        someip::SomeIpConfiguration someipConfig{someip::TransportProtocol::TCP, someip::EndUserType::CLIENT};
        std::shared_ptr<someip::someipConnection> clientInstance = someip::someipConnection::SetSomeIpConfiguration(io_service, port, someipConfig);

        public:

            SomeIpNetworkBinding(string ip, int port);
            SomeIpNetworkBinding(int service_id, int instance_id, string ip, int port);

            void OfferService() override;

            template <typename... Params>
            void SendRequest(uint32_t methodID, Params... args);
            template <typename T>
            void ReceiveMessage(T &in);

            static vector<serviceinfo> FindService_SomeIp(int serviceID, ara::com::InstanceIdentifier instance_id = 0xffff);
        };
    }
}
