#pragma once

#include "types.hpp"
//#include "../../com/language_binding/skeleton/include/serviceskeleton.hpp"
//#include "../../com/include/types.hpp"

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            class PackageManagerSkeleton //: public ara::com::skeleton::skeletonBase
            {
            public:
                /** TransferStart **/
                struct TransferStartInput
                {
                    uint64_t Size;
                };
                ///////////////////////////// PROBLEM //////////////////////////////////
                struct TransferStartOutput
                {
                    TransferIdType id;                       // Size (in bytes) of the Software Package to be transferred
                    uint32_t BlockSize;                      // Size of the blocks to be received with TransferData method
                    OperationResultType TransferStartResult; // Success or Failure
                };

                /** TransferData **/
                struct TransferDataInput
                {
                    TransferIdType id;
                    ByteVectorType data;
                    uint64_t blockCounter;
                };

                struct TransferDataOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** TransferExit **/
                struct TransferExitInput
                {
                    TransferIdType id;
                };

                struct TransferExitOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** TransferDelete **/
                struct TransferDeleteInput
                {
                    TransferIdType id;
                };

                struct TransferDeleteOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** ProcessSwPackage **/
                struct ProcessSwPackageInput
                {
                    TransferIdType id;
                };

                struct ProcessSwPackageOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** Activate **/
                struct ActivateOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };
                ///////////////////////////// PROBLEM //////////////////////////////////
                /** GetSwClusterInfo **/
                struct GetSwClusterInfoOutput
                {
                    vector<ara::ucm::SwClusterInfoType> vectorOfClusterInfo;
                };

                /** Rollback **/
                struct RollbackOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** RevertProcessedSwPackages **/
                struct RevertProcessedSwPackagesOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** Finish **/
                struct FinishOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                struct GetCurrentStatusField
                {
                    PackageManagerStatusType Status;
                };

                PackageManagerSkeleton () = default;

                /* PACKAGE MANAGER METHODS */

                virtual std::future<TransferStartOutput> TransferStart(uint64_t Size) = 0;

                virtual std::future<TransferDataOutput> TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter) = 0;

                virtual std::future<TransferExitOutput> TransferExit(TransferIdType &id) = 0;

                virtual std::future<TransferDeleteOutput> TransferDelete(TransferIdType &id) = 0;

                virtual std::future<ProcessSwPackageOutput> ProcessSwPackage(TransferIdType &id) = 0;

                virtual std::future<ActivateOutput> Activate() = 0;

                virtual std::future<GetSwClusterInfoOutput> GetSwClusterInfo() = 0;

                virtual std::future<RollbackOutput> Rollback() = 0;

                virtual std::future<RevertProcessedSwPackagesOutput> RevertProcessedSwPackages() = 0;

                virtual std::future<FinishOutput> Finish() = 0;

                virtual std::future<GetCurrentStatusField> GetCurrentStatus() = 0;

                /*inline void handleMethod(int methodID) override
                {
                    switch (methodID)
                    {
                    case 1:
                    {
                        TransferStartInput ip;
                        this->ptr2bindingProtocol->ReceiveMessage(ip);
                        std::future<TransferStartOutput> op = TransferStart(ip.Size);
                        this->ptr2bindingProtocol->SendRequest(1, op.get());
                        break;
                    }
                    case 2:
                    {
                        TransferDataInput ip;
                        this->ptr2bindingProtocol->ReceiveMessage(ip);
                        std::future<TransferDataOutput> op = TransferData(ip.id, ip.data, ip.blockCounter);
                        this->ptr2bindingProtocol->SendRequest(2, op.get());
                        break;
                    }
                    case 3:
                    {
                        TransferExitInput ip;
                        this->ptr2bindingProtocol->ReceiveMessage(ip);
                        std::future<TransferExitOutput> op = TransferExit(ip.id);
                        this->ptr2bindingProtocol->SendRequest(3, op.get());
                        break;
                    }
                    case 4:
                    {
                        TransferDeleteInput ip;
                        this->ptr2bindingProtocol->ReceiveMessage(ip);
                        std::future<TransferDeleteOutput> op = TransferDelete(ip.id);
                        this->ptr2bindingProtocol->SendRequest(4, op.get());
                        break;
                    }
                    case 5:
                    {
                        ProcessSwPackageInput ip;
                        this->ptr2bindingProtocol->ReceiveMessage(ip);
                        std::future<ProcessSwPackageOutput> op = ProcessSwPackage(ip.id);
                        this->ptr2bindingProtocol->SendRequest(5, op.get());
                        break;
                    }
                    case 6:
                    {
                        std::future<ActivateOutput> op = Activate();
                        this->ptr2bindingProtocol->SendRequest(6, op.get());
                        break;
                    }
                    case 7:
                    {
                        std::future<GetSwClusterInfoOutput> op = GetSwClusterInfo();
                        this->ptr2bindingProtocol->SendRequest(7, op.get());
                        break;
                    }
                    case 8:
                    {
                        std::future<RollbackOutput> op = Rollback();
                        this->ptr2bindingProtocol->SendRequest(8, op.get());
                        break;
                    }
                    case 9:
                    {
                        std::future<RevertProcessedSwPackagesOutput> op = RevertProcessedSwPackages();
                        this->ptr2bindingProtocol->SendRequest(9, op.get());
                        break;
                    }
                    case 10:
                    {
                        std::future<FinishOutput> op = Finish();
                        this->ptr2bindingProtocol->SendRequest(10, op.get());
                        break;
                    }
                    case 11:
                    {
                        std::future<GetCurrentStatusField> f = GetCurrentStatus();
                        this->ptr2bindingProtocol->SendRequest(11, f.get());
                        break;
                    }
                    } 
                } */
            };
        }
    }
}
