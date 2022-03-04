#pragma once

#include "UCM_Common.hpp"
#include "State.hpp"
#include "com/COM_Types.hpp"
#include <future> 



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
                    TransferIdType id;		  // Size (in bytes) of the Software Package to be transferred
			        uint32_t BlockSize;		  // Size of the blocks to be received with TransferData method
			        OperationResultType TransferStartResult; // Success or Failure
                };

                struct OperationResultOutput {
                    ara::ucm::OperationResultType OperationReturn;
                };

                struct GetSWClusterInfoOutput{
                    vector<ara::ucm::SwClusterInfoType> vectorOfClusterInfo;
                };

                struct GetCurrentStatusOutput{
                    PackageManagerStatusType Status;
                };

                
                /* PACKAGE MANAGER METHODS */
                virtual std::future<TransferStartOutput> TransferStart(uint64_t Size) = 0;

                virtual ara::ucm::OperationResultType  TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter) = 0;
                
                virtual std::future<OperationResultOutput> TransferExit(TransferIdType &id) = 0;

                virtual std::future<OperationResultOutput> TransferDelete(TransferIdType &id) = 0;

                virtual std::future<OperationResultOutput> ProcessSwPackage(TransferIdType &id) = 0;

                virtual std::future<OperationResultOutput> Activate() = 0;

                virtual std::future<GetSWClusterInfoOutput> GetSwClusterInfo() = 0;

                virtual std::future<OperationResultOutput> Rollback() = 0;

                virtual std::future<OperationResultOutput> RevertProcessedSwPackages() = 0;

                virtual std::future<GetCurrentStatusOutput> GetCurrentStatus() = 0;

                virtual std::future<OperationResultOutput> Finish() = 0;

                private:

		    	};
            }
        }
    }
