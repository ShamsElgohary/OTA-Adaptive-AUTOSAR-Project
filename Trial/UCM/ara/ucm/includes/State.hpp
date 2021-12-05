#pragma once

#include "UCM_Common.hpp"
#include "Storage.hpp"
#include "SynchronizedStorage.hpp"
#include "Parsing.hpp"



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
                PackageManagerStatusType *CurrentStatus;

                //inline static uint8_t ProcessListVersion = 0;

                enum class SM_ReceivedStates : uint8_t
                {
                    kRejected = 5U,
                    kVerifyFailed = 6U,
                    kPrepareFailed = 7U,
                    kRollbackFailed = 8U,
                };

            public:
                PackageManagerState(PackageManagerStatusType &pkgmgr_CurrentStatus);
                PackageManagerState();
                void Activate();
                void Cancel(ara::ucm::TransferIdType);
                void Finish();
                void GetStatus();
                ara::ucm::OperationResultType ProcessSwPackage(TransferIdType &id);
                void RevertProcessedSwPackages();
                void Rollback();
                void dependencyCheck(void);
            };
        }
    }
}