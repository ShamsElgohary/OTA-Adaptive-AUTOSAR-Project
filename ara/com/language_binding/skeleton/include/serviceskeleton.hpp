#pragma once 
#include <iostream>

#include "../../../include/types.hpp"
#include "../../../network_binding/network_binding_base.hpp"

using namespace std ;

namespace ara 
{
    namespace com
    {
        namespace skeleton 
        {
            class Serviceskeleton
            {
                protected:
                    ara::com::serviceIdentifierType;
                    uint32_t VersionMajor;
                    uint32_t VersionMinor;

                    std::map<ara::com::InstanceIdentifier, ara::com::InterfaceIdentifier> InstancesMap;
                    
                    std::map<ara::com::InterfaceIdentifier, std::shared_ptr <ara::com::NetworkBase>> network_bindingMap;
                
                public:
                    /* MUST (Implementation can either be from generator or not) */
                    Serviceskeleton(ara::com::InstanceIdentifier instanceId,
                        ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent);

                    /* EXTRA (Implementation can either be from generator or not) */
                    Serviceskeleton(ara::com::InstanceIdentifierContainer instanceIds,
                        ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent);

                    /* EXTRA (Implementation can either be from generator or not) */
                    Serviceskeleton(ara::core::InstanceSpecifier instanceSpec,
                        ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent);

                    Serviceskeleton(const Serviceskeleton& other) = delete;

                    Serviceskeleton& operator=(const Serviceskeleton& other) = delete;

                    ~Serviceskeleton();

                    /* MUST */
                    void OfferService();
                    
                    /* MUST */
                    void StopOfferService();
            };
        }
    }
}
