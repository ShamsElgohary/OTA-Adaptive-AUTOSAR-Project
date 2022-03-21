#include <iostream>
#include <memory>
#include <string>
#include "NetworkBinding.hpp"
#include "types.hpp"
#include "SomeIp/servicediscovery/lib/include/servicediscovery.hpp"
using namespace std;

namespace ara
{
    namespace com
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// SOMEIP NETWORK BINDING ///////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        SomeIpNetworkBinding::SomeIpNetworkBinding(string ip ,int port)
        {
            this->port = port;
            this->ip = ip;
        }
        template <typename... Params>
        void SomeIpNetworkBinding::send(Params... args)
        {

        }
        template <typename... Params>
        void SomeIpNetworkBinding::receive(Params& ...args)
        {

        }
        template <typename... Params>
        void SomeIpNetworkBinding::SendRequest(uint32_t methodID, Params... args)
        {

        }
        void SomeIpNetworkBinding::OfferService()
        {
          servicediscovery::offer_service(service_id, instance_id, ip,port );
        }
        vector<ara::com::SomeIpNetworkBinding::output> SomeIpNetworkBinding::FindService_SomeIp(int serviceID ,ara::com::InstanceIdentifier instance_id = 0xffff)
        {
            servicediscovery::find_service(service_id,instance_id);

        }
    }
}
