#pragma once
#include "types.hpp"
#include "../com/language_binding/skeleton/include/serviceskeleton.hpp"

namespace ara
{
    namespace sm
    {
        class UpdateRequestSkeleton : public ara::com::skeleton::skeletonBase
        {
        public:
            /** StartUpdateSession **/
            struct StartUpdateSessionOutput
            {
                uint8_t AppError;
            };

            /** PrepareUpdate **/
            struct PrepareUpdateInput
            {
                FunctionGroupList FunctionGroups;
            };

            struct PrepareUpdateOutput
            {
                uint8_t AppError;
            };

            /** VerifyUpdate **/
            struct VerifyUpdateInput
            {
                FunctionGroupList FunctionGroups;
            };

            struct VerifyUpdateOutput
            {
                uint8_t AppError;
            };

            /** StopUpdateSession **/
            struct StopUpdateSessionOutput
            {
                uint8_t AppError;
            };

            virtual std::future<StartUpdateSessionOutput> StartUpdateSession() = 0;
            virtual std::future<PrepareUpdateOutput> PrepareUpdate(FunctionGroupList FunctionGroups) = 0;
            virtual std::future<VerifyUpdateOutput> VerifyUpdate(FunctionGroupList FunctionGroups) = 0;
            virtual void StopUpdateSession() = 0;

            UpdateRequestSkeleton(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : skeletonBase(1, I_id, mode)
            {
            }
            inline void handleMethod() override
            {}
            /*inline void handleMethod(int methodID) override
            {
                switch (methodID)
                {
                case 1:
                    StartUpdateSessionOutput op = StartUpdateSession();
                    this->ptr2bindingProtocol->send(op);
                    break;
                case 2:
                    PrepareUpdateInput ip;
                    this->ptr2bindingProtocol->receive(ip);
                    PrepareUpdateOutput op = PrepareUpdate(ip.FunctionGroups);
                    this->ptr2bindingProtocol->send(op);
                    break;
                case 3:
                    VerifyUpdateInput ip;
                    this->ptr2bindingProtocol->receive(ip);
                    VerifyUpdateOutput op = VerifyUpdate(ip.FunctionGroups);
                    this->ptr2bindingProtocol->send(op);
                    break;
                case 4:
                    StopUpdateSessionOutput op = StopUpdateSession();
                    this->ptr2bindingProtocol->send(op);
                    break;
                }
            }
            */
        };
    }
}