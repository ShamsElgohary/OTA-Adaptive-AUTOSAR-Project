#pragma once
#include <iostream>

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
            class UCM_Skeleton
            {

            public:
                /* MUST (Implementation can either be from generator or not) */
                UCM_Skeleton(ara::com::InstanceIdentifier instanceId,
                             ara::com::MethodCallProcessingMode mode);

                /* EXTRA (Implementation can either be from generator or not) */
                UCM_Skeleton(ara::com::InstanceIdentifierContainer instanceIds,
                             ara::com::MethodCallProcessingMode mode);

                /* EXTRA (Implementation can either be from generator or not) */
                UCM_Skeleton(ara::core::InstanceSpecifier instanceSpec,
                             ara::com::MethodCallProcessingMode mode);

                UCM_Skeleton(const UCM_Skeleton &other) = delete;

                UCM_Skeleton &operator=(const UCM_Skeleton &other) = delete;

                ~UCM_Skeleton();

                /* MUST */
                void OfferService();

                /* MUST */
                void StopOfferService();

                void setID(ara::com::InstanceIdentifier id);
                void setMode(ara::com::MethodCallProcessingMode mode);

                struct TransferStartOutput
                {
                    uint32_t id;
                    uint32_t BlockSize;
                    bool TransferStartResult;
                };

                class startTransfer : public method::methodBase {
                    void processMethod(uint64_t size)
                    {
                        TransferStart(size);
                    }
                };

                class ayHaga : public method::methodBase {
                    void processMethod(ayArgument ayArgument)
                    {
                        TransferStart(ayArgument);
                    }
                };

                ara::core::Future<TransferStartOutput> SoftwarePackage::TransferStart(uint64_t Size);

            private:
                ara::com::InstanceIdentifier instanceId;
                
            };
        }
    }
}
