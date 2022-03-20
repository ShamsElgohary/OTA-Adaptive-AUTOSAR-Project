#pragma once
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include "types.hpp"

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
            struct output
            {
                std::shared_ptr<NetworkBindingBase> networkBindingPtr;
                int instanceID;
                string ip;
                int port;
                int serviceID;
            };
            static std::shared_ptr<NetworkBindingBase> Bind(int serviceId, int instanceId);
            template <typename... Params>
            void send(Params... args);
            template <typename T>
            void receive(T &in);
            template <typename... Params>
            void SendRequest(uint32_t methodID, Params... args);
            void OfferService();
            void StopOfferService();
            int get_method_id();
        };
        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// SOMEIP NETWORK BINDING ///////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        class SomeIpNetworkBinding : public NetworkBindingBase
        {
            int port;
            string ip;

        public:
            SomeIpNetworkBinding(int port, string ip);
            template <typename... Params>
            void send(Params... args) override;
            template <typename T>
            void receive(T &in) override;
            template <typename... Params>
            void SendRequest(uint32_t methodID, Params... args) override;
            static vector<ara::com::NetworkBindingBase::output> FindService_SomeIp(int serviceID);
        };
    }
}
