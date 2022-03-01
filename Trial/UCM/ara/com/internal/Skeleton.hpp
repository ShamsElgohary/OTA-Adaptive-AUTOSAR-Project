#ifndef _ARA_COM_INTERNAL_SKELETON_HPP_
#define  _ARA_COM_INTERNAL_SKELETON_HPP_

#include "COM_Types.hpp"
#include "CORE_Types.hpp"

namespace ara
{
    namespace com
    {
        namespace internal
        {
            class Skeleton {

                public:
                    Skeleton(ara::com::InstanceIdentifier instanceId,
                        ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent);

                    Skeleton(ara::com::InstanceIdentifierContainer instanceIds,
                        ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent);

                    Skeleton(ara::core::InstanceSpecifier instanceSpec,
                        ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent);

                    Skeleton(const Skeleton& other) = delete;

                    Skeleton& operator=(const Skeleton& other) = delete;

                    ~Skeleton();

                    void OfferService();
                    
                    void StopOfferService();
            };
        }
    }
}

#endif