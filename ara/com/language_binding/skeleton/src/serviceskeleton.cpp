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
            Serviceskeleton::Serviceskeleton(ara::com::InstanceIdentifier instanceId,ara::com::MethodCallProcessingMode mode =MethodCallProcessingMode::kEvent)
            {
                this->instanceID = instanceId;
                this->mode = mode;
                //parse manifest using instanceID to get rest of info
                //this->port = instanceManifest.port;
                //this->ip = instanceManifest.ip;
                //ptr2bindingprotocol
                this->ptr2bindingProtocol=std::make_shared<NetworkBindingBase>();
                // if configured as someip
                //this->ptr2bindingProtocol=std::make_shared<SomeIpNetworkBinding>(this->serviceID,this->instanceID,this->port,this->ip);
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
                this->ptr2bindingProtocol->OfferService();
                std::thread serve(serve);
            }

            void Serviceskeleton::StopOfferService()
            {
                this->ptr2bindingProtocol->StopOfferService();
                stopOfferFlag = true;
            }

        }
    }
}


 handleMethod(methodID , input)
 {
     switch(methodID)
     {
         case 3:
         {

         }
     }
 }
 