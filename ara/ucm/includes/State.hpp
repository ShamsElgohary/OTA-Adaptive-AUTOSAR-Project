#pragma once

#include "UCM_Common.hpp"
#include "UpdateRequestProxy.hpp"

#define PrepareUpdateCounter 3
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
                PackageManagerStatusType CurrentStatus;
                uint8_t SWPackagesCounter=0;
                static uint16_t ProcessListVersion;
                //shared ptr creation
                std::shared_ptr <ara::sm::UpdateRequest::proxy::UpdateRequestproxy> proxy = nullptr;
                ara::log guiLogger;


            public:
                PackageManagerState(PackageManagerStatusType pkgmgr_CurrentStatus);
                PackageManagerState();
                ara::ucm::OperationResultType ActivateInternal();
                ara::ucm::OperationResultType Cancel(ara::ucm::TransferIdType);
                ara::ucm::OperationResultType FinishInternal();
                PackageManagerStatusType GetStatusInternal();
                ara::ucm::OperationResultType ProcessSwPackageInternal(TransferIdType id);
                ara::ucm::OperationResultType RevertProcessedSwPackages();
                ara::ucm::OperationResultType RollbackInternal();
                void DependencyCheck(void);
            };
        }
    }
}