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

enum class PackageManagerStatusType : uint8_t 
{
kCleaningUp = 7U, 
kRolledBack = 6U, 
kRollingBack = 5U, 
kActivated = 4U, 
kActivating = 3U, 
kProcessing = 2U, 
kReady = 1U, 
kIdle = 0U, 
kVerifying = 8U, 
};
enum class OperationResultType : uint8_t 
{
kSuccess = 0U, 
kInsufficientMemory = 1U, 
kIncorrectBlock = 2U, 
kIncorrectSize = 3U, 
kInvalidTransferId = 4U, 
kOperationNotPermitted = 5U, 
kIncorrectBlockSize = 30U, 
kInsufficientData = 6U, 
};
using TransferIdType = std::array<uint8_t, 16>;
using ByteVectorType = std::vector<uint8_t>;
using SwNameType = std::string;
using StrongRevisionLabelString = std::string;
enum class SwClusterStateType : uint8_t 
{
kUpdated = 2U, 
kAdded = 1U, 
kPresent = 0U, 
kRemoved = 3U, 
};
struct SwClusterInfoType{
	SwNameType Name;
	StrongRevisionLabelString Version;
	SwClusterStateType State;
private:
	template <typename Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar &Name;
		ar &Version;
		ar &State;
	}
	friend class boost::serialization::access;
};
using SwClusterInfoVectorType = std::vector<SwClusterInfoType>;

                    struct FinishOutput
                    {
                        OperationResultType Result;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &Result;
                        }
                        friend class boost::serialization::access;
                    };

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

                    struct TransferStartOutput
                    {
                        TransferIdType id;

                        uint32_t BlockSize;

                        OperationResultType TransferStartResult;

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
                        OperationResultType OperationReturn;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &OperationReturn;
                        }
                        friend class boost::serialization::access;
                    };

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
                        OperationResultType Result;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &Result;
                        }
                        friend class boost::serialization::access;
                    };

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
                        OperationResultType Result;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &Result;
                        }
                        friend class boost::serialization::access;
                    };

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
                        OperationResultType Result;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &Result;
                        }
                        friend class boost::serialization::access;
                    };

                    struct ActivateOutput
                    {
                        OperationResultType Result;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &Result;
                        }
                        friend class boost::serialization::access;
                    };

                    struct GetSwClusterInfoOutput
                    {
                        SwClusterInfoVectorType Result;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &Result;
                        }
                        friend class boost::serialization::access;
                    };

                    struct RollbackOutput
                    {
                        OperationResultType Result;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &Result;
                        }
                        friend class boost::serialization::access;
                    };

                    struct RevertProcessedSwPackagesOutput
                    {
                        OperationResultType Result;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &Result;
                        }
                        friend class boost::serialization::access;
                    };

                    struct CurrentStatusField
                    {
                        PackageManagerStatusType CurrentStatus;
                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &CurrentStatus;
                        }
                        friend class boost::serialization::access;
                    };

                class PackageManagerSkeleton : public ara::com::skeleton::skeletonBase
                {
                public:
                    PackageManagerSkeleton(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : skeletonBase(CUSTOMIZED_PROJECT_PATH + "executables/ucm/0.1/etc/service_manifest.json", 2, I_id, Cluster_Name, mode)
                    {}
                    virtual std::future<FinishOutput> Finish () = 0;
                    virtual std::future<TransferStartOutput> TransferStart (uint64_t Size) = 0;
                    virtual std::future<TransferDataOutput> TransferData (TransferIdType id , ByteVectorType data , uint64_t blockCounter) = 0;
                    virtual std::future<TransferExitOutput> TransferExit (TransferIdType id) = 0;
                    virtual std::future<TransferDeleteOutput> TransferDelete (TransferIdType id) = 0;
                    virtual std::future<ProcessSwPackageOutput> ProcessSwPackage (TransferIdType id) = 0;
                    virtual std::future<ActivateOutput> Activate () = 0;
                    virtual std::future<GetSwClusterInfoOutput> GetSwClusterInfo () = 0;
                    virtual std::future<RollbackOutput> Rollback () = 0;
                    virtual std::future<RevertProcessedSwPackagesOutput> RevertProcessedSwPackages () = 0;
                    virtual std::future<CurrentStatusField> GetCurrentStatus () = 0;

                    void handleMethod() override
                    {

                        int methodID;
                        string methodName;
                        stringstream payload = this->ptr2bindingProtocol->ReceiveMessage(methodID);
                    switch (methodID)
                    {

                        case 10:
                        {
                        methodName = "Finish" ;
                        std::future<FinishOutput> F_op = Finish();

                        Serializer2 S;
                        stringstream result;
                        FinishOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(10, result);
                        break;
                        }


                        case 1:
                        {
                        methodName = "TransferStart" ;
                        TransferStartInput ip ;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<TransferStartOutput> F_op = TransferStart(ip.Size) ;


                        Serializer2 S;
                        stringstream result;
                        TransferStartOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(1, result);
                        break;
                        }


                        case 2:
                        {
                        methodName = "TransferData" ;
                        TransferDataInput ip ;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<TransferDataOutput> F_op = TransferData(ip.id , ip.data , ip.blockCounter) ;


                        Serializer2 S;
                        stringstream result;
                        TransferDataOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(2, result);
                        break;
                        }


                        case 3:
                        {
                        methodName = "TransferExit" ;
                        TransferExitInput ip ;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<TransferExitOutput> F_op = TransferExit(ip.id) ;


                        Serializer2 S;
                        stringstream result;
                        TransferExitOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(3, result);
                        break;
                        }


                        case 4:
                        {
                        methodName = "TransferDelete" ;
                        TransferDeleteInput ip ;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<TransferDeleteOutput> F_op = TransferDelete(ip.id) ;


                        Serializer2 S;
                        stringstream result;
                        TransferDeleteOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(4, result);
                        break;
                        }


                        case 5:
                        {
                        methodName = "ProcessSwPackage" ;
                        ProcessSwPackageInput ip ;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<ProcessSwPackageOutput> F_op = ProcessSwPackage(ip.id) ;


                        Serializer2 S;
                        stringstream result;
                        ProcessSwPackageOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(5, result);
                        break;
                        }


                        case 6:
                        {
                        methodName = "Activate" ;
                        std::future<ActivateOutput> F_op = Activate();

                        Serializer2 S;
                        stringstream result;
                        ActivateOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(6, result);
                        break;
                        }


                        case 7:
                        {
                        methodName = "GetSwClusterInfo" ;
                        std::future<GetSwClusterInfoOutput> F_op = GetSwClusterInfo();

                        Serializer2 S;
                        stringstream result;
                        GetSwClusterInfoOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(7, result);
                        break;
                        }


                        case 8:
                        {
                        methodName = "Rollback" ;
                        std::future<RollbackOutput> F_op = Rollback();

                        Serializer2 S;
                        stringstream result;
                        RollbackOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(8, result);
                        break;
                        }


                        case 9:
                        {
                        methodName = "RevertProcessedSwPackages" ;
                        std::future<RevertProcessedSwPackagesOutput> F_op = RevertProcessedSwPackages();

                        Serializer2 S;
                        stringstream result;
                        RevertProcessedSwPackagesOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(9, result);
                        break;
                        }

                        case 11:
                        {
                        methodName = "GetCurrentStatus" ;
                        std::future<CurrentStatusField> F_op = GetCurrentStatus();
                        Serializer2 S;
                        stringstream result;
                        CurrentStatusField op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(11, result);
                        break;
                        }
                    }
                        ara::com::AddMethodCall(methodID, methodName, ara::com::MethodType::Skeleton_Method, 2, Cluster_Name);
                }
            };
        }
    }
}
