#pragma once
#include "types.hpp"
#include "serviceskeleton.hpp"
#include "serialization_simple.hpp"
using namespace std;
namespace ara
{
    namespace sm
    {
        namespace UpdateRequest
        {

enum class SM_ApplicationError : uint8_t 
{
kPrepared = 1U, 
kVerified = 1U, 
kRejected = 5U, 
kVerifyFailed = 6U, 
kPrepareFailed = 7U, 
kRollbackFailed = 8U, 
};
using Functiongroup = std::string;
using FunctionGroupList = std::vector<Functiongroup>;

                    struct StartUpdateSessionOutput
                    {
                        SM_ApplicationError AppError;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &AppError;
                        }
                        friend class boost::serialization::access;
                    };

                    struct PrepareUpdateInput
                    {
                        FunctionGroupList FunctionGroups;
                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &FunctionGroups;
                        }
                        friend class boost::serialization::access;
                    };

                    struct PrepareUpdateOutput
                    {
                        SM_ApplicationError AppError;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &AppError;
                        }
                        friend class boost::serialization::access;
                    };

                    struct VerifyUpdateInput
                    {
                        FunctionGroupList FunctionGroups;
                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &FunctionGroups;
                        }
                        friend class boost::serialization::access;
                    };

                    struct VerifyUpdateOutput
                    {
                        SM_ApplicationError AppError;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &AppError;
                        }
                        friend class boost::serialization::access;
                    };

                class UpdateRequestSkeleton : public ara::com::skeleton::skeletonBase
                {
                public:
                    UpdateRequestSkeleton(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : skeletonBase(CUSTOMIZED_PROJECT_PATH + "executables/ucm/0.1/etc/service_manifest.json", 1, I_id, Cluster_Name, mode)
                    {}
                    virtual std::future<StartUpdateSessionOutput> StartUpdateSession () = 0;
                    virtual std::future<PrepareUpdateOutput> PrepareUpdate (FunctionGroupList FunctionGroups) = 0;
                    virtual std::future<VerifyUpdateOutput> VerifyUpdate (FunctionGroupList FunctionGroups) = 0;
                    virtual std::future<StopUpdateSessionOutput> StopUpdateSession () = 0;

                    void handleMethod() override
                    {

                        int methodID;
                        string methodName;
                        stringstream payload = this->ptr2bindingProtocol->ReceiveMessage(methodID);
                    switch (methodID)
                    {

                        case 1:
                        {
                        methodName = "StartUpdateSession" ;
                        std::future<StartUpdateSessionOutput> F_op = StartUpdateSession();

                        Serializer2 S;
                        stringstream result;
                        StartUpdateSessionOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(1, result);
                        break;
                        }


                        case 2:
                        {
                        methodName = "PrepareUpdate" ;
                        PrepareUpdateInput ip ;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<PrepareUpdateOutput> F_op = PrepareUpdate(ip.FunctionGroups) ;


                        Serializer2 S;
                        stringstream result;
                        PrepareUpdateOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(2, result);
                        break;
                        }


                        case 3:
                        {
                        methodName = "VerifyUpdate" ;
                        VerifyUpdateInput ip ;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<VerifyUpdateOutput> F_op = VerifyUpdate(ip.FunctionGroups) ;


                        Serializer2 S;
                        stringstream result;
                        VerifyUpdateOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(3, result);
                        break;
                        }


                        case 4:
                        {
                        methodName = "StopUpdateSession" ;
                        std::future<StopUpdateSessionOutput> F_op = StopUpdateSession();

                        Serializer2 S;
                        stringstream result;
                        StopUpdateSessionOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(4, result);
                        break;
                        }
                    }
                        ara::com::AddMethodCall(methodID, methodName, ara::com::MethodType::Skeleton_Method, 1, Cluster_Name);
                }
            };
        }
    }
}
