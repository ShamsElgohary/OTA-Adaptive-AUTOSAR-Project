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

        SomeIpNetworkBinding::SomeIpNetworkBinding(serviceIdentifierType service_id, InstanceIdentifier instance_id,
                                                   string ip, uint16_t port, someip::EndUserType type) : ip{ip}, port{port},
                                                                                                         serviceId{service_id}, InstanceId{instance_id}, someipConfig{someip::TransportProtocol::TCP, type}
        {
        }
        void SomeIpNetworkBinding::SendRequest(uint32_t methodID, stringstream &s)
        {
            if (!clientInstance)
            {
                clientInstance = someip::someipConnection::SetSomeIpConfiguration(io_service, port, someipConfig);
            }
            someip::someipHeader header(serviceId, methodID);
            someip::someipMessage someipMsg(header, s);
            clientInstance->SendMessage(someipMsg);
        }

        void SomeIpNetworkBinding::CloseConnection()
        {
            clientInstance->CloseConnection();
            clientInstance = nullptr;
        }

        void SomeIpNetworkBinding::ServerListen()
        {
            if (!clientInstance)
            {
                clientInstance = someip::someipConnection::SetSomeIpConfiguration(io_service, port, someipConfig);
            }
            clientInstance->ServerListen();
        }
        stringstream SomeIpNetworkBinding::ReceiveMessage(int &method_id)
        {

            someip::someipMessage someipMsg = clientInstance->ReceiveMessage();
            method_id = someipMsg.header.getMethodID();
            std::stringstream receivedData;
            receivedData << someipMsg.payload;
            return receivedData;
        }
        stringstream SomeIpNetworkBinding::ReceiveMessage()
        {
            someip::someipMessage someipMsg = clientInstance->ReceiveMessage();
            std::stringstream receivedData;
            receivedData << someipMsg.payload;
            return receivedData;
        }

        void SomeIpNetworkBinding::OfferService()
        {
            servicediscovery::offer_service(serviceId, InstanceId, ip, port);
        }

        vector<serviceinfo> SomeIpNetworkBinding::FindService_SomeIp(int serviceID, ara::com::InstanceIdentifier instance_id)
        {
            return servicediscovery::find_service(serviceID, instance_id);
        }

        void SomeIpNetworkBinding::StopOfferService()
        {
            servicediscovery::stop_offer_service(serviceId, InstanceId);
        }
    }
}
