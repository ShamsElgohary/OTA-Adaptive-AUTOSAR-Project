#pragma once

#include "types.hpp"
//#include "../../com/language_binding/skeleton/include/serviceskeleton.hpp"

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            class PackageManagerSkeleton //: public ara::com::skeleton::Serviceskeleton
            {
            public:
                /** TransferStart **/
                struct TransferStartInput
                {
                    uint64_t Size;
                };

                struct TransferStartOutput
                {
                    TransferIdType id;                       // Size (in bytes) of the Software Package to be transferred
                    uint32_t BlockSize;                      // Size of the blocks to be received with TransferData method
                    OperationResultType TransferStartResult; // Success or Failure
                };

                /** TransferData **/
                struct TransferDataInput
                {
                    TransferIdType &id;
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
                    TransferIdType &id;
                };

                struct TransferExitOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** TransferDelete **/
                struct TransferDeleteInput
                {
                    TransferIdType &id;
                };

                struct TransferDeleteOutput
                {
                    ara::ucm::OperationResultType OperationReturn;
                };

                /** ProcessSwPackage **/
                struct ProcessSwPackageInput
                {
                    TransferIdType &id;
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

                
                struct GetCurrentStatusField{
                    PackageManagerStatusType Status;
                };

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
/*
                inline void handleMethod(int methodID) override
                {
                    switch (methodID)
                    {
                    case 1:
                        TransferStartInput ip;
                        this->ptr2bindingProtocol->receive(ip);
                        TransferStartOutput op = TransferStart(ip.Size);
                        this->ptr2bindingProtocol->send(op);
                        break;
                    case 2:
                        TransferDataInput ip;
                        this->ptr2bindingProtocol->receive(ip);
                        TransferDataOutput op = TransferData(ip.id, ip.data, ip.blockCounter);
                        this->ptr2bindingProtocol->send(op);
                        break;
                    case 3:
                        TransferExitInput ip;
                        this->ptr2bindingProtocol->receive(ip);
                        TransferExitOutput op = TransferExit(ip.id);
                        this->ptr2bindingProtocol->send(op);
                        break;
                    case 4:
                        TransferDeleteInput ip;
                        this->ptr2bindingProtocol->receive(ip);
                        TransferDeleteOutput op = TranferDelete(ip.id);
                        this->ptr2bindingProtocol->send(op);
                        break;
                    case 5:
                        ProcessSwPackageInput ip;
                        this->ptr2bindingProtocol->receive(ip);
                        ProcessSwPackageOutput op = ProcessSwPackage(ip.id);
                        this->ptr2bindingProtocol->send(op);
                        break;
                    case 6:
                        ActivateOutput op = Activate();
                        this->ptr2bindingProtocol->send(op);
                        break;
                    case 7:
                        GetSwClusterInfoOutput op = GetSwClusterInfo();
                        this->ptr2bindingProtocol->send(op);
                        break;
                    case 8:
                        RollbackOutput op = Rollback();
                        this->ptr2bindingProtocol->send(op);
                        break;
                    case 9:
                        RevertProcessedSwPackagesOutput op = RevertProcessedSwPackages();
                        this->ptr2bindingProtocol->send(op);
                        break;
                    case 10:
                        FinishOutput op = Finish();
                        this->ptr2bindingProtocol->send(op);
                        break;
                    case 11:
                        GetCurrentStatusField f = GetCurrentStatus();
                        this->ptr2bindingProtocol->send(f);
                        break;
                    }
                }
*/
            };
        }
    }
}
