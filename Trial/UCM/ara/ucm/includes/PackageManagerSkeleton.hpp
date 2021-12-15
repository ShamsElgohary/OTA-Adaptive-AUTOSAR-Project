#pragma once

#include "UCM_Common.hpp"
#include "State.hpp"

#include "com/comTypes.hpp"
#include "core/Future.hpp"

namespace ara
{
    namespace com
    {
        namespace skeleton
        {
                class PackageManagerSkeleton 
                {
                public:

                PackageManagerSkeleton(ara::com::InstanceIdentifier instanceId,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent);

                /* DESTRUCTOR */
                ~PackageManagerSkeleton();

                /*  NO COPY CONSTRUCTOR */
                PackageManagerSkeleton(const PackageManagerSkeleton& other) = delete;

                /* INSTANCES NOT ASSIGNABLE */
                PackageManagerSkeleton& operator=(const PackageManagerSkeleton& other) = delete;
                
                /* OFFER SERVER INSTANCE */
                void OfferService();
                
                /* STOP OFFERING SERVICE */
                void StopOfferService();

                //fields::CurrentStatus CurrentStatus;

                /* For all output and non-void return parameters an enclosing struct is generated */
                struct TransferStartOutput {
                    ara::ucm::TransferStartReturnType StartReturn;
                };

                
                /* PACKAGE MANAGER METHODS */
                virtual ara::core::Future<TransferStartOutput> TransferStart(uint64_t Size) = 0;



                virtual ara::ucm::OperationResultType  TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter) = 0;
                
                virtual ara::ucm::OperationResultType TransferExit(TransferIdType &id) = 0;

                virtual ara::ucm::OperationResultType TransferDelete(TransferIdType &id) = 0;

                virtual ara::ucm::OperationResultType ProcessSwPackage(TransferIdType &id) = 0;

                virtual ara::ucm::OperationResultType Activate() = 0;

                virtual vector<ara::ucm::SwClusterInfoType> GetSwClusterInfo() = 0;

                virtual ara::ucm::OperationResultType Rollback() = 0;

                virtual ara::ucm::OperationResultType RevertProcessedSwPackages() = 0;

                virtual PackageManagerStatusType GetCurrentStatus() = 0;

                private:

		    	};
            }
        }
    }
