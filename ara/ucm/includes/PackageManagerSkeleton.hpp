#pragma once

#include "types.hpp"
#include "serviceskeleton.hpp"
#include "serialization_simple.hpp"

using namespace std;

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            class PackageManagerSkeleton : public ara::com::skeleton::skeletonBase
            {
            public:
                PackageManagerSkeleton(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : 
                skeletonBase("/home/yasmin/Desktop/Graduation_Project/02-OurImpement/OTA-Adaptive-AUTOSAR-Project/executables/ucm/etc/service_manifest.json",2, I_id, mode)
                {}
                /** TransferStart **/
                struct TransferStartInput
                {
                    uint64_t Size;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &Size;
                    }
                    friend class boost::serialization::access;
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
                        ar &TransferStartResult;
                    }
                    friend class boost::serialization::access;
                };

                /** TransferData **/
                struct TransferDataInput
                {
                    TransferIdType id;
                    ByteVectorType data;
                    uint64_t blockCounter;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &id;
                        ar &data;
                        ar &blockCounter;
                    }
                    friend class boost::serialization::access;
                };

                struct TransferDataOutput
                {
                    ara::ucm::OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                /** TransferExit **/
                struct TransferExitInput
                {
                    TransferIdType id;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &id;
                    }
                    friend class boost::serialization::access;
                };

                struct TransferExitOutput
                {
                    ara::ucm::OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                /** TransferDelete **/
                struct TransferDeleteInput
                {
                    TransferIdType id;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &id;
                    }
                    friend class boost::serialization::access;
                };

                struct TransferDeleteOutput
                {
                    ara::ucm::OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };

                /** ProcessSwPackage **/
                struct ProcessSwPackageInput
                {
                    TransferIdType id;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &id;
                    }
                    friend class boost::serialization::access;
                };

                struct ProcessSwPackageOutput
                {
                    ara::ucm::OperationResultType OperationReturn;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &OperationReturn;
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
                        ar &OperationReturn;
                    }
                    friend class boost::serialization::access;
                };
                ///////////////////////////// PROBLEM //////////////////////////////////
                /** GetSwClusterInfo **/
                struct GetSwClusterInfoOutput
                {
                    vector<SwClusterInfoType> vectorOfClusterInfo;

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
                        ar &OperationReturn;
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
                        ar &OperationReturn;
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
                        ar &OperationReturn;
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

                virtual std::future<TransferStartOutput> TransferStart(uint64_t Size) = 0;

                virtual std::future<TransferDataOutput> TransferData(TransferIdType id, ByteVectorType data, uint64_t blockCounter) = 0;

                virtual std::future<TransferExitOutput> TransferExit(TransferIdType id) = 0;

                virtual std::future<TransferDeleteOutput> TransferDelete(TransferIdType id) = 0;

                virtual std::future<ProcessSwPackageOutput> ProcessSwPackage(TransferIdType id) = 0;

                virtual std::future<ActivateOutput> Activate() = 0;

                virtual std::future<GetSwClusterInfoOutput> GetSwClusterInfo() = 0;

                virtual std::future<RollbackOutput> Rollback() = 0;

                virtual std::future<RevertProcessedSwPackagesOutput> RevertProcessedSwPackages() = 0;

                virtual std::future<FinishOutput> Finish() = 0;

                virtual std::future<GetCurrentStatusField> GetCurrentStatus() = 0;

                void handleMethod() override
                {
                    int methodID;
                    string methodName;
                    this->ptr2bindingProtocol->ServerListen();
                    stringstream payload = this->ptr2bindingProtocol->ReceiveMessage(methodID);
                    cout << "[UCM Skeleton] handle: " << methodID << endl;
                    switch (methodID)
                    {
                    case 1:
                    {
                        methodName = "TransferStart";
                        TransferStartInput ip;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<TransferStartOutput> F_op = TransferStart(ip.Size);

                        Serializer2 S;
                        stringstream result;
                        TransferStartOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(1, result);

                        break;
                    }
                    case 2:
                    {
                        methodName = "TransferData";
                        TransferDataInput ip;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<TransferDataOutput> F_op = TransferData(ip.id, ip.data, ip.blockCounter);

                        Serializer2 S;
                        stringstream result;
                        TransferDataOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(2, result);

                        break;
                    }
                    case 3:
                    {
                        methodName = "TransferExit";
                        TransferExitInput ip;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<TransferExitOutput> F_op = TransferExit(ip.id);

                        Serializer2 S;
                        stringstream result;
                        TransferExitOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(3, result);

                        break;
                    }
                    case 4:
                    {
                        methodName = "TransferDelete";
                        TransferDeleteInput ip;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<TransferDeleteOutput> F_op = TransferDelete(ip.id);

                        Serializer2 S;
                        stringstream result;
                        TransferDeleteOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(4, result);

                        break;
                    }
                    case 5:
                    {
                        methodName = "ProcessSwPackage";
                        ProcessSwPackageInput ip;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<ProcessSwPackageOutput> F_op = ProcessSwPackage(ip.id);

                        Serializer2 S;
                        stringstream result;
                        ProcessSwPackageOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(5, result);

                        break;
                    }
                    case 6:
                    {
                        methodName = "Activate";
                        std::future<ActivateOutput> F_op = Activate();

                        Serializer2 S;
                        stringstream result;
                        ActivateOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(6, result);

                        break;
                    }
                    case 7:
                    {
                        methodName = "GetSwClusterInfo";
                        std::future<GetSwClusterInfoOutput> F_op = GetSwClusterInfo();

                        Serializer2 S;
                        stringstream result;
                        GetSwClusterInfoOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(7, result);

                        break;
                    }
                    case 8:
                    {
                        methodName = "Rollback";
                        std::future<RollbackOutput> F_op = Rollback();

                        Serializer2 S;
                        stringstream result;
                        RollbackOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(8, result);

                        break;
                    }
                    case 9:
                    {
                        methodName = "RevertProcessedSwPackages";
                        std::future<RevertProcessedSwPackagesOutput> F_op = RevertProcessedSwPackages();

                        Serializer2 S;
                        stringstream result;
                        RevertProcessedSwPackagesOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(9, result);

                        break;
                    }
                    case 10:
                    {
                        methodName = "Finish";
                        std::future<FinishOutput> F_op = Finish();

                        Serializer2 S;
                        stringstream result;
                        FinishOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(10, result);

                        break;
                    }
                    case 11:
                    {
                        methodName = "GetCurrentStatus";
                        std::future<GetCurrentStatusField> F_op = GetCurrentStatus();

                        Serializer2 S;
                        stringstream result;
                        GetCurrentStatusField op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendRequest(11, result);

                        break;
                    }
                    }
                    this->ptr2bindingProtocol -> CloseConnection();
                    ara::com::AddMethodCall (methodID, methodName, ara::com::MethodType::Skeleton_Method, 2);
                }
            };
        }
    }
}
