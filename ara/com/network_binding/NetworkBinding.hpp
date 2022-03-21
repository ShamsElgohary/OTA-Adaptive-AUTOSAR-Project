#pragma once
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include "../include/types.hpp"

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
            void send(Params... args);
            template <typename T>
            void receive(T &in);
            template <typename... Params>
            void SendRequest(uint32_t methodID, Params... args);
            virtual void OfferService();
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
            struct output
            {
                string ip;
                int port;
                int instance_id;

            };
            int service_id;
            int instance_id;
            string ip;
            int port ;
            SomeIpNetworkBinding(string ip, int port):ip{ip},port{port}{}
            SomeIpNetworkBinding(int service_id, int instance_id,string ip, int port):ip{ip},port{port}{}

            void OfferService() override ;

            template <typename... Params>
            void send(Params... args) override;

            template <typename... Params>
            void receive(Params& ...args) override;

            template <typename... Params>
            void SendRequest(uint32_t methodID, Params... args) override;

            static vector<ara::com::SomeIpNetworkBinding::output> FindService_SomeIp(int serviceID ,ara::com::InstanceIdentifier instance_id = 0xffff);
        };
    }
}
