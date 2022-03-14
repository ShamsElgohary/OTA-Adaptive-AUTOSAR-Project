#pragma once
#include <iostream>
#include <thread>
#include <utility>
#include <map>
#include "../../../include/types.hpp"
#include "../../../network_binding/network_binding_base.hpp"
#include "method.hpp"

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
                ara::com::serviceIdentifierType serviceID;
                ara::com::InstanceIdentifier instanceID;
                ara::com::MethodCallProcessingMode mode;
                std::shared_ptr<ara::com::NetworkBindingBase> ptr2bindingProtocol;
                uint32_t port;
                uint32_t ip;
                map<uint32_t, method::methodBase *> ID2method;
                void serve(void);
                //void processMethod(uint32_t); //return type to be determined (betragga3 arguments el "send")

            public:
                /* MUST (Implementation can either be from generator or not) */
                Serviceskeleton(ara::com::InstanceIdentifier instanceId,
                                ara::com::MethodCallProcessingMode mode =
                                    ara::com::MethodCallProcessingMode::kEvent);

                Serviceskeleton(const Serviceskeleton &other) = delete;

                Serviceskeleton &operator=(const Serviceskeleton &other) = delete;

                virtual ~Serviceskeleton();

                /* MUST */
                void OfferService();

                /* MUST */
                void StopOfferService();

                static void handleMethod(method::methodBase *, method::input ip);
            };
        }
    }
}
