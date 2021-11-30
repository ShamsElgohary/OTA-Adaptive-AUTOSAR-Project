#pragma once

#include "UCM_Common.hpp"
#include "includes/Storage.hpp"



namespace ara
{
    namespace ucm
    {
        namespace state
        {
            class PackageManagerState
            {
            private:

                /* State of The Entire UCM PKG */ 
                inline static PackageManagerStatusType CurrentStatus = PackageManagerStatusType::kIdle;

                inline static uint8_t ProcessListVersion = 0;

                enum class SM_ReceivedStates : uint8_t
                {
                    kRejected = 5U,
                    kVerifyFailed = 6U,
                    kPrepareFailed = 7U,
                    kRollbackFailed = 8U,
                };

            public:
                void Activate();
                void Cancel(ara::ucm::TransferIdType);
                void Finish();
                void GetStatus();
                void ProcessSwPackage(ara::ucm::TransferIdType);
                void RevertProcessedSwPackages();
                void Rollback();
                void dependencyCheck(void);
            };
        }
    }
}