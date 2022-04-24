#pragma once
#include "types.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include "../com/language_binding/skeleton/include/serviceskeleton.hpp"
#include "serialization_simple.hpp"

namespace ara
{
    namespace sm
    {
        namespace skeleton
        {
            class UpdateRequest : public ara::com::skeleton::skeletonBase
            {
            public:
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

            virtual std::future<StartUpdateSessionOutput> StartUpdateSession() = 0;
            virtual std::future<PrepareUpdateOutput> PrepareUpdate(FunctionGroupList FunctionGroups) = 0;
            virtual std::future<VerifyUpdateOutput> VerifyUpdate(FunctionGroupList FunctionGroups) = 0;
            virtual void StopUpdateSession() = 0;

            UpdateRequestSkeleton(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : skeletonBase(1, I_id, mode)
            {
            }
           
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

                virtual uint8_t StartUpdateSession() = 0;
                virtual uint8_t PrepareUpdate(FunctionGroupList FunctionGroups) = 0;
                virtual uint8_t VerifyUpdate(FunctionGroupList FunctionGroups) = 0;
                virtual uint8_t StopUpdateSession() = 0;

                void handleMethod() override
                {
                    int methodID;
                    this->ptr2bindingProtocol->ServerListen();
                    stringstream payload = this->ptr2bindingProtocol->ReceiveMessage(methodID);
                    Deserializer2 D;
                    Serializer2 S;
                    stringstream result;

                    PrepareUpdateInput prepareUpdateInput;
                    StartUpdateSessionOutput startUpdateSessionOutput;
                    PrepareUpdateOutput prepareUpdateOutput;
                    VerifyUpdateInput verifyUpdateInput;
                    VerifyUpdateOutput verifyUpdateOutput;
                    StopUpdateSessionOutput stopUpdateSessionOutput;

                    switch (methodID)
                    {
                    case 1:
                        startUpdateSessionOutput.AppError = StartUpdateSession();
                        S.serialize(result, startUpdateSessionOutput);
                        this->ptr2bindingProtocol->SendRequest(1, result);
                        break;
                    case 2:
                        D.deserialize(payload, prepareUpdateInput);
                        prepareUpdateOutput.AppError = PrepareUpdate(prepareUpdateInput.FunctionGroups);
                        S.serialize(result, prepareUpdateOutput);
                        this->ptr2bindingProtocol->SendRequest(2, result);
                        break;
                    case 3:
                        D.deserialize(payload, verifyUpdateInput);
                        verifyUpdateOutput.AppError = VerifyUpdate(verifyUpdateInput.FunctionGroups);
                        S.serialize(result, verifyUpdateOutput);
                        this->ptr2bindingProtocol->SendRequest(3, result);
                        break;
                    case 4:
                        stopUpdateSessionOutput.AppError = StopUpdateSession();
                        S.serialize(result, stopUpdateSessionOutput);
                        this->ptr2bindingProtocol->SendRequest(4, result);
                        break;
                    }
                }
            };
        }
    }
}