#pragma once
#include <iostream>

#include "../../../include/types.hpp"
#include "../../../network_binding/network_binding_base.hpp"

using namespace std;

namespace ara
{
    namespace com
    {
        namespace skeleton
        {
            class Serviceskeleton
            {
            private:
                ara::com::InstanceIdentifier instanceID;
                ara::com::MethodCallProcessingMode mode;
                std::shared_ptr<ara::com::NetworkBase> ptr2bindingProtocol;
                uint32_t port;
                uint32_t ip;
                void processMethod(uint32_t); //return type to be determined (betragga3 arguments el "send")

            public:
                /* MUST (Implementation can either be from generator or not) */
                Serviceskeleton(ara::com::InstanceIdentifier instanceId,
                                ara::com::MethodCallProcessingMode mode =
                                    ara::com::MethodCallProcessingMode::kEvent);

                Serviceskeleton(const Serviceskeleton &other) = delete;

                Serviceskeleton &operator=(const Serviceskeleton &other) = delete;

                ~Serviceskeleton();

                /* MUST */
                void OfferService();

                /* MUST */
                void StopOfferService();
            };
        }
    }
}
