#pragma once
#include <iostream>
#include "serviceskeleton.hpp"
#include "types.hpp"

using namespace std;

bool stopOfferFlag = false;

namespace ara
{
    namespace com
    {
        namespace skeleton
        {
            Serviceskeleton::Serviceskeleton(ara::com::InstanceIdentifier instanceId, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent)
            {
                this->instanceID = instanceId;
                this->mode = mode;
                this->ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(serviceID, instanceID, port);
            }
            //to be generated
            void Serviceskeleton::handleMethod(int id)
            {
                switch (id)
                {
                case 1:
                    input_add in;
                    this->ptr2bindingProtocol->receive(in);
                    int out = add(in.i, in.y);
                    this->ptr2bindingProtocol->send(out);
                    break;
                }
            }
            void Serviceskeleton::serve()
            {
                while (1)
                {
                    if (stopOfferFlag == true)
                    {
                        break;
                    }
                    handleMethod(this->ptr2bindingProtocol->get_method_id());
                }
            }
            void Serviceskeleton::OfferService()
            {
                this->ptr2bindingProtocol->OfferService();
                std::thread t(serve);
            }
            void Serviceskeleton::StopOfferService()
            {
                this->ptr2bindingProtocol->StopOfferService();
                stopOfferFlag = true;
            }
        }
    }
}
