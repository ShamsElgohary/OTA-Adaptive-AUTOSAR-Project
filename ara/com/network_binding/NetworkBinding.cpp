#include "NetworkBinding.hpp"

using namespace std;

namespace ara
{
    namespace com
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// SOMEIP NETWORK BINDING ///////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        SomeIpNetworkBinding::SomeIpNetworkBinding(string ip, int port) : ip{ip}, port{port} {}

        SomeIpNetworkBinding::SomeIpNetworkBinding(int service_id, int instance_id, string ip, int port) : ip{ip}, port{port}, service_id{service_id}, instance_id{instance_id} {}

        template <typename... Params>
        void SomeIpNetworkBinding::send(Params... args)
        {
        }
        template <typename... Params>
        void SomeIpNetworkBinding::receive(Params &...args)
        {
        }
        template <typename... Params>
        void SomeIpNetworkBinding::SendRequest(uint32_t methodID, Params... args)
        {
        }
        void SomeIpNetworkBinding::OfferService()
        {
            servicediscovery::offer_service(service_id, instance_id, ip, port);
        }
        vector<serviceinfo> SomeIpNetworkBinding::FindService_SomeIp(int serviceID, ara::com::InstanceIdentifier instance_id)
        {
            return servicediscovery::find_service(serviceID, instance_id);
        }
    }
}
