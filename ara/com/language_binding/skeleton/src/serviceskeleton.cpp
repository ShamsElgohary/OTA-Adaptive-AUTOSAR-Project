#pragma once
#include <iostream>
#include "../include/serviceskeleton.hpp"
using namespace std;

bool stopOfferFlag = false;

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

            void Serviceskeleton::handleMethod(method::methodBase *ptr, method::input ip)
            {
                method::output op = ptr->method::methodBase::processMethod(ip);
                this->ptr2bindingProtocol->send(op); //replying to service consumer
            }

            void Serviceskeleton::serve()
            {
                while (1)
                {
                    if (stopOfferFlag == true)
                    {
                        break;
                    }
                    std::pair<uint32_t, method::input> receivedPair = this->ptr2bindingProtocol->receive();
                    //uint32_t methodID = this->ptr2bindingProtocol->receive(); //gowaha listen to port
                    std::thread handler(Serviceskeleton::handleMethod, this->ID2method[receivedPair.first], std::ref(receivedPair.second));
                }
            }

            void Serviceskeleton::OfferService()
            {
                //uniqueness check
                this->ptr2bindingProtocol->SD::OfferService(this->serviceID, this->instanceId, this->ip, this->port);
                std::thread server(serve);
            }

            void Serviceskeleton::StopOfferService()
            {
                this->ptr2bindingProtocol->SD::StopOfferService(this->serviceID, this->instanceID);
                stopOfferFlag = true;
            }

        }
    }
}
