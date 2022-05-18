#pragma once

#include "types.hpp"
#include "serviceskeleton.hpp"
#include "serialization_simple.hpp"

using namespace std;

namespace ara
{
    namespace sm
    {
        namespace skeleton
        {
            class UpdateRequestSkeleton : public ara::com::skeleton::skeletonBase
            {
            public:
               UpdateRequestSkeleton(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : skeletonBase("/home/kareem/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/executables/sm/etc/service_manifest.json",1 , I_id, Cluster_Name, mode)
               {
               }
                /** StartUpdateSession **/
                struct StartUpdateSessionOutput
                {
                    uint8_t AppError;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &AppError;
                    }
                    friend class boost::serialization::access;
                };

                /** PrepareUpdate **/
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
                    uint8_t AppError;

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
                    uint8_t AppError;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &AppError;
                    }
                    friend class boost::serialization::access;
                };

                /** StopUpdateSession **/
                struct StopUpdateSessionOutput
                {
                    uint8_t AppError;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &AppError;
                    }
                    friend class boost::serialization::access;
                };

            virtual std::future<StartUpdateSessionOutput> StartUpdateSession() = 0;
            virtual std::future<PrepareUpdateOutput> PrepareUpdate(FunctionGroupList FunctionGroups) = 0;
            virtual std::future<VerifyUpdateOutput> VerifyUpdate(FunctionGroupList FunctionGroups) = 0;
            virtual void StopUpdateSession() = 0;

                void handleMethod() override
                {
                    int methodID;
                    string methodName;
                    this->ptr2bindingProtocol->ServerListen();
                    stringstream payload = this->ptr2bindingProtocol->ReceiveMessage(methodID);
                    Deserializer2 D;
                    Serializer2 S;
                    stringstream result;

                    PrepareUpdateInput prepareUpdateInput;
                    std::future<StartUpdateSessionOutput> startUpdateSessionOutput;
                    std::future<PrepareUpdateOutput> prepareUpdateOutput;
                    VerifyUpdateInput verifyUpdateInput;
                    std::future<VerifyUpdateOutput> verifyUpdateOutput;
                    std::future<StopUpdateSessionOutput> stopUpdateSessionOutput;
                    cout << "[SM Skeleton] handle: " << methodID << endl;
                    switch (methodID)
                    {
                    case 1:
                    {
                        methodName = "StartUpdateSession";
                        startUpdateSessionOutput = StartUpdateSession();
                        StartUpdateSessionOutput out1=startUpdateSessionOutput.get();
                        S.serialize(result, out1);
                        this->ptr2bindingProtocol->SendRequest(1, result);
                        break;
                    }
                    case 2:
                    {
                        methodName = "PrepareUpdate";
                        D.deserialize(payload, prepareUpdateInput);
                        prepareUpdateOutput = PrepareUpdate(prepareUpdateInput.FunctionGroups);
                        PrepareUpdateOutput out2=prepareUpdateOutput.get();
                        S.serialize(result, out2);
                        this->ptr2bindingProtocol->SendRequest(2, result);
                        break;
                    } 
                    case 3:
                    {
                        methodName = "VerifyUpdate";
                        D.deserialize(payload, verifyUpdateInput);
                        verifyUpdateOutput = VerifyUpdate(verifyUpdateInput.FunctionGroups);
                        VerifyUpdateOutput out3=verifyUpdateOutput.get();
                        S.serialize(result, out3);
                        this->ptr2bindingProtocol->SendRequest(3, result);
                        break;
                    }
                    case 4:
                    {
                        methodName = "StopUpdateSession";
                        /*fire and forget function*/
                        StopUpdateSession();
                        //stopUpdateSessionOutput = StopUpdateSession();
                        //S.serialize(result, stopUpdateSessionOutput);
                        this->ptr2bindingProtocol->SendRequest(4, result);
                        break;
                    }
                    }
                    this->ptr2bindingProtocol -> CloseConnection();
                    ara::com::AddMethodCall (methodID, methodName, ara::com::MethodType::Skeleton_Method, 1, Cluster_Name);
                }
            };
        }
    }
}