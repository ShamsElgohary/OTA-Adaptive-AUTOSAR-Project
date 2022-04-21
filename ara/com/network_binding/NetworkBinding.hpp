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
            template <typename... Params>
            void SendRequest(uint32_t methodID, Params... args){}
            template <typename T>
            void ReceiveMessage(T &in){}

            virtual void OfferService(){}
            virtual void StopOfferService(){}
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
        someip::SomeIpConfiguration someipConfig;
        std::shared_ptr<someip::someipConnection> clientInstance ;

        public:

            SomeIpNetworkBinding(string ip, uint16_t port);
            SomeIpNetworkBinding(int service_id, int instance_id, string ip, uint16_t port ,someip::EndUserType type);

            void OfferService() override;
            void StopOfferService()override;

            template <typename... Params>
            void SendRequest(uint32_t methodID, Params... args);
            template <typename T>
            void ReceiveMessage(T &in);

            static vector<serviceinfo> FindService_SomeIp(int serviceID, ara::com::InstanceIdentifier instance_id = 0xffff);
        };
    }
}
