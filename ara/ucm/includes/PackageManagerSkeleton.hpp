#pragma once

#include "types.hpp"
#include "../../com/language_binding/skeleton/include/serviceskeleton.hpp"
#include "../../com/include/types.hpp"

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            class PackageManagerSkeleton : public ara::com::skeleton::skeletonBase
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
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &id;
                        ar &BlockSize;
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
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

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** TransferExit **/
                struct TransferExitInput
                {
                    TransferIdType id;
                };

                struct TransferExitOutput
                {
                    ara::ucm::OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** TransferDelete **/
                struct TransferDeleteInput
                {
                    TransferIdType id;
                };

                struct TransferDeleteOutput
                {
                    ara::ucm::OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** ProcessSwPackage **/
                struct ProcessSwPackageInput
                {
                    TransferIdType id;
                };

                struct ProcessSwPackageOutput
                {
                    ara::ucm::OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** Activate **/
                struct ActivateOutput
                {
                    ara::ucm::OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };
                ///////////////////////////// PROBLEM //////////////////////////////////
                /** GetSwClusterInfo **/
                struct GetSwClusterInfoOutput
                {
                    vector<ara::ucm::SwClusterInfoType> vectorOfClusterInfo;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &vectorOfClusterInfo;
                    }
                    friend class boost::serialization::access;
                };

                /** Rollback **/
                struct RollbackOutput
                {
                    ara::ucm::OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** RevertProcessedSwPackages **/
                struct RevertProcessedSwPackagesOutput
                {
                    ara::ucm::OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                /** Finish **/
                struct FinishOutput
                {
                    ara::ucm::OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationResultType;
                    }
                    friend class boost::serialization::access;
                };

                struct GetCurrentStatusField
                {
                    PackageManagerStatusType Status;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &Status;
                    }
                    friend class boost::serialization::access;
                };

                /* PACKAGE MANAGER METHODS */

                virtual TransferStartOutput TransferStart(uint64_t Size) = 0;

                virtual TransferDataOutput TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter) = 0;

                virtual TransferExitOutput TransferExit(TransferIdType &id) = 0;

                virtual TransferDeleteOutput TransferDelete(TransferIdType &id) = 0;

                virtual ProcessSwPackageOutput ProcessSwPackage(TransferIdType &id) = 0;

                virtual ActivateOutput Activate() = 0;

                virtual GetSwClusterInfoOutput GetSwClusterInfo() = 0;

                virtual RollbackOutput Rollback() = 0;

                virtual RevertProcessedSwPackagesOutput RevertProcessedSwPackages() = 0;

                virtual FinishOutput Finish() = 0;

                virtual GetCurrentStatusField GetCurrentStatus() = 0;

                inline void handleMethod(int methodID) override
                {
                    switch (methodID)
                    {
                    case 1:
                    {
                        TransferStartInput ip;
                        this->ptr2bindingProtocol->ReceiveMessage(ip);
                        TransferStartOutput op = TransferStart(ip.Size);
                        this->ptr2bindingProtocol->SendRequest(1, op);
                        break;
                    }
                    case 2:
                    {
                        TransferDataInput ip;
                        this->ptr2bindingProtocol->ReceiveMessage(ip);
                        TransferDataOutput op = TransferData(ip.id, ip.data, ip.blockCounter);
                        this->ptr2bindingProtocol->SendRequest(2, op);
                        break;
                    }
                    case 3:
                    {
                        TransferExitInput ip;
                        this->ptr2bindingProtocol->ReceiveMessage(ip);
                        TransferExitOutput op = TransferExit(ip.id);
                        this->ptr2bindingProtocol->SendRequest(3, op);
                        break;
                    }
                    case 4:
                    {
                        TransferDeleteInput ip;
                        this->ptr2bindingProtocol->ReceiveMessage(ip);
                        TransferDeleteOutput op = TransferDelete(ip.id);
                        this->ptr2bindingProtocol->SendRequest(4, op);
                        break;
                    }
                    case 5:
                    {
                        ProcessSwPackageInput ip;
                        this->ptr2bindingProtocol->ReceiveMessage(ip);
                        ProcessSwPackageOutput op = ProcessSwPackage(ip.id);
                        this->ptr2bindingProtocol->SendRequest(5, op);
                        break;
                    }
                    case 6:
                    {
                        ActivateOutput op = Activate();
                        this->ptr2bindingProtocol->SendRequest(6, op);
                        break;
                    }
                    case 7:
                    {
                        GetSwClusterInfoOutput op = GetSwClusterInfo();
                        this->ptr2bindingProtocol->SendRequest(7, op);
                        break;
                    }
                    case 8:
                    {
                        RollbackOutput op = Rollback();
                        this->ptr2bindingProtocol->SendRequest(8, op);
                        break;
                    }
                    case 9:
                    {
                        RevertProcessedSwPackagesOutput op = RevertProcessedSwPackages();
                        this->ptr2bindingProtocol->SendRequest(9, op);
                        break;
                    }
                    case 10:
                    {
                        FinishOutput op = Finish();
                        this->ptr2bindingProtocol->SendRequest(10, op);
                        break;
                    }
                    case 11:
                    {
                        GetCurrentStatusField f = GetCurrentStatus();
                        this->ptr2bindingProtocol->SendRequest(11, f);
                        break;
                    }
                    }
                }
            };
        }
    }
}
