#pragma once
#include <iostream>
#include <thread>
#include <utility>
#include <map>
#include "types.hpp"
#include "NetworkBinding.hpp"

using namespace std;

namespace ara
{
    namespace com
    {
        namespace skeleton
        {
            class Serviceskeleton
            {
            public:
                ara::com::serviceIdentifierType serviceID{1};
                ara::com::InstanceIdentifier instanceID;
                ara::com::MethodCallProcessingMode mode;
                std::shared_ptr<ara::com::NetworkBindingBase> ptr2bindingProtocol;
                uint32_t port;
                string ip;
                void serve();
                virtual void handleMethod(int methodID);
                Serviceskeleton(ara::com::InstanceIdentifier instanceId, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent);
                Serviceskeleton(const Serviceskeleton &other) = delete;
                Serviceskeleton &operator=(const Serviceskeleton &other) = delete;
                virtual ~Serviceskeleton();
                void OfferService();
                void StopOfferService();
            };
        }
    }
}