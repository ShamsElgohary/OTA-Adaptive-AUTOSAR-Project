#pragma once
#include "types.hpp"
#include "serviceskeleton.hpp"
#include "serialization_simple.hpp"
using namespace std;
namespace carcontrol
{

                class CarControlSkeleton : public ara::com::skeleton::skeletonBase
                {
                public:
                    CarControlSkeleton(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : skeletonBase(CUSTOMIZED_PROJECT_PATH + "executables/ucm/0.1/etc/service_manifest.json", 3, I_id, Cluster_Name, mode)
                    {}
                    struct ProcessCarActionInput
                    {
                        CarActionType Action;
                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &Action;
                        }
                        friend class boost::serialization::access;
                    };

                    struct ProcessCarActionOutput
                    {
                        CarResultType Result;

                    private:
                        template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                        {
                        ar &Result;
                        }
                        friend class boost::serialization::access;
                    };

                    virtual std::future<ProcessCarActionOutput> ProcessCarAction (CarActionType Action) = 0;

                    void handleMethod() override
                    {

                        int methodID;
                        string methodName;
                        stringstream payload = this->ptr2bindingProtocol->ReceiveMessage(methodID);
                    switch (methodID)
                    {

                        case 1:
                        {
                        methodName = "ProcessCarAction" ;
                        ProcessCarActionInput ip ;
                        Deserializer2 D;
                        D.deserialize(payload, ip);
                        std::future<ProcessCarActionOutput> F_op = ProcessCarAction(ip.Action) ;

                        Serializer2 S;
                        stringstream result;
                        ProcessCarActionOutput op = F_op.get();
                        S.serialize(result, op);
                        this->ptr2bindingProtocol->SendResponse(1, result);
                        break;
                        }
                    }
                        ara::com::AddMethodCall(methodID, methodName, ara::com::MethodType::Skeleton_Method, 3, Cluster_Name);
                }
            };
}
