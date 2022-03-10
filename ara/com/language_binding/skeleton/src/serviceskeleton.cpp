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
            void Serviceskeleton::OfferService()
            {
                //uniqueness check
                this->ptr2bindingProtocol->OfferService(this->instanceId, this->port, this->ip);
                while (1)
                {
                    uint32_t methodID = this->ptr2bindingProtocol->receive(); //gowaha listen to port
                    this->ptr2bindingProtocol->send(processMethod(methodID)); //replying to service consumer
                }
            }

            void Serviceskeleton::StopOfferService()
            {
                this->ptr2bindingProtocol->StopOfferService(this->instanceID);
            }

        }
    }
}
