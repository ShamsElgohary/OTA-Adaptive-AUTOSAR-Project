#pragma once
#include <iostream>
#include "../include/serviceskeleton.hpp"
using namespace std;

namespace ara
{
    namespace com
    {
        namespace skeleton
        {
            Serviceskeleton(ara::com::InstanceIdentifier instanceId,
                            ara::com::MethodCallProcessingMode mode =
                                ara::com::MethodCallProcessingMode::kEvent)
            {
                this->instanceID = instanceId;
                this->mode = mode;
                //parse manifest using instanceID to get rest of info
                //this->port = instanceManifest.port;
                //this->ip = instanceManifest.ip;
                //ptr2bindingprotocol
            }

            void handleMethod(method::methodBase *ptr, method::input ip)
            {
                method::output op = ptr->processMethod(ip);
                this->ptr2bindingProtocol->send(op); //replying to service consumer
            }

            void Serviceskeleton::OfferService()
            {
                //uniqueness check
                this->ptr2bindingProtocol->OfferService(this->instanceId, this->port, this->ip);
                while (1)
                {
                    std::pair<uint32_t, method::input> receivedPair = this->ptr2bindingProtocol->receive();
                    //uint32_t methodID = this->ptr2bindingProtocol->receive(); //gowaha listen to port
                    if (receivedPair.first == 5000)
                    {
                        break;
                    }
                    std::thread handler(Serviceskeleton::handleMethod, this->ID2method[receivedPair.first], std::ref(receivedPair.second));
                }
            }

            void Serviceskeleton::StopOfferService()
            {
                this->ptr2bindingProtocol->StopOfferService(this->instanceID);
            }

        }
    }
}
