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

        SomeIpNetworkBinding::SomeIpNetworkBinding(int service_id, int instance_id, string ip, int port) :
                    ip{ip}, port{port}, serviceId{service_id}, InstanceId{instance_id} {}



        template <typename... Params>
        void SomeIpNetworkBinding::SendRequest(uint32_t methodID, Params... args)
        {
            std::stringstream payload;
            Serializer S;
            S.Serialize(payload,std::forward<Params>(args)...);
		    someip::someipHeader header(serviceId, methodId);
            someip::someipMessage someipMsg(header,payload);

            clientInstance->SendRequest(someipMsg);
        }
        
        template <typename T>
        void SomeIpNetworkBinding::ReceiveMessage(T &in)
        {
            someip::someipMessage someipMsg = clientInstance->ReceiveMessage();
            std::stringstream payload;
            payload << someipMsg.payload;
            Deserializer D;
        }


        void SomeIpNetworkBinding::OfferService()
        {
            servicediscovery::offer_service(serviceId, InstanceId, ip, port);
        }

        vector<serviceinfo> SomeIpNetworkBinding::FindService_SomeIp(int serviceID, ara::com::InstanceIdentifier instance_id)
        {
            return servicediscovery::find_service(serviceID, instance_id);
        }

    }
}
