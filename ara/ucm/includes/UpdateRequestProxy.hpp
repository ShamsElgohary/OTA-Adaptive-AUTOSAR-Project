#pragma once

#include "../../sm/types.hpp"
#include "../com/language_binding/proxy/include/serviceProxy.hpp"
#include "../com/language_binding/proxy/include/method.hpp"
namespace ara
{
    namespace sm
    {
        namespace proxy
        {
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

            namespace methods
            {
                class StartUpdateSession : public ara::com::proxy::method::MethodBase <StartUpdateSessionOutput>
                {
                };

                class PrepareUpdate : public ara::com::proxy::method::MethodBase <PrepareUpdateOutput, FunctionGroupList>
                {
                };

                class VerifyUpdate : public ara::com::proxy::method::MethodBase <VerifyUpdateOutput, FunctionGroupList>
                {
                };

                class StopUpdateSession : public ara::com::proxy::method::MethodBase <StopUpdateSessionOutput>
                {
                };
            }

            class UpdateRequestProxy // : public ara::com::proxy::ProxyBase
            {
            public:
                inline ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService()
                {
                    ara::com::proxy::ProxyBase::FindService(2);
                }

                inline ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(ara::com::InstanceIdentifier InstanceID)
                {
                    ara::com::proxy::ProxyBase::FindService(2, InstanceID);
                }

                methods::StartUpdateSession StartUpdateSession;
                methods::PrepareUpdate PrepareUpdate;
                methods::VerifyUpdate VerifyUpdate;
                methods::StopUpdateSession StopUpdateSession;
            };
        }
    }
}
