#include "NetworkBinding.hpp"

using namespace std;

namespace ara
{
    namespace com
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// SOMEIP NETWORK BINDING ///////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        SomeIpNetworkBinding::SomeIpNetworkBinding(string ip, uint16_t port) : ip{ip}, port{port} {}

        SomeIpNetworkBinding::SomeIpNetworkBinding(int service_id, int instance_id, string ip, uint16_t port ,someip::EndUserType type) :
                    ip{ip}, port{port}, serviceId{service_id}, InstanceId{instance_id},someipConfig{someip::TransportProtocol::TCP, type},
                    clientInstance{someip::someipConnection::SetSomeIpConfiguration(io_service, port, someipConfig)} {
                        

                    }



        template <typename... Params>
        void SomeIpNetworkBinding::SendRequest(uint32_t methodID, Params... args)
        {
            std::stringstream payload;
            Serializer S;
            S.Serialize(payload,std::forward<Params>(args)...);
		    someip::someipHeader header(serviceId, methodID);
            someip::someipMessage someipMsg(header,payload);

            clientInstance->SendRequest(someipMsg);
        }
        
        template <typename T>
        void SomeIpNetworkBinding::ReceiveMessage(T &in)
        {
            someip::someipMessage someipMsg = clientInstance->ReceiveMessage();
            std::stringstream receivedData;
            receivedData << someipMsg.payload;
            Deserializer D;
            D.Deserialize(receivedData, &in);
        }

        void SomeIpNetworkBinding::OfferService()
        {
            servicediscovery::offer_service(serviceId, InstanceId, ip, port);
        }

        vector<serviceinfo> SomeIpNetworkBinding::FindService_SomeIp(int serviceID, ara::com::InstanceIdentifier instance_id)
        {
            return servicediscovery::find_service(serviceID, instance_id);
        }

        int NetworkBindingBase::get_method_id()
        {
            // std::cout << "GET_METHOD_ID" << std::endl;
            return 1;
        }

        void SomeIpNetworkBinding::StopOfferService()
        {
            servicediscovery::stop_offer_service(serviceId,InstanceId);
        }

    }
}
