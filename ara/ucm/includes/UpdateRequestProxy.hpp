#pragma once

#include "../../sm/types.hpp"
#include "serviceProxy.hpp"
#include "method.hpp"
#include "types.hpp"
namespace ara
{
    namespace sm
    {
        namespace UpdateRequest
        {
            namespace proxy
            {
                using Functiongroup = std::string;
                using FunctionGroupList = std::vector<Functiongroup>;
                enum class SM_ApplicationError : uint8_t
                {
                    kPrepared = 1U,
                    kVerified = 2U,
                    kRejected = 5U,
                    kVerifyFailed = 6U,
                    kPrepareFailed = 7U,
                    kRollbackFailed = 8U,
                };
                /** StartUpdateSession **/
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
                    SM_ApplicationError AppError;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &AppError;
                    }
                    friend class boost::serialization::access;
                };

                /** VerifyUpdate **/
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

                /** StopUpdateSession **/
                struct StopUpdateSessionOutput
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

                namespace methods
                {
                    class StartUpdateSession : public ara::com::proxy::method::MethodBase
                    {
                        public:
                        StartUpdateSession(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 1) {}
                        StartUpdateSessionOutput operator()()
                        {
                            StartUpdateSessionOutput out;
                            process_method_call<StartUpdateSessionOutput>(out);
                            return out;
                        }
                    };

                    class PrepareUpdate : public ara::com::proxy::method::MethodBase
                    {
                        public:
                        PrepareUpdate(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 2) {}
                        PrepareUpdateOutput operator()(FunctionGroupList fg)
                        {
                            PrepareUpdateInput in;
                            in.FunctionGroups =fg ;
                            PrepareUpdateOutput out;
                            process_method_call<PrepareUpdateOutput, PrepareUpdateInput>(in, out);
                            return out;
                        }
                    };

                    class VerifyUpdate : public ara::com::proxy::method::MethodBase
                    {
                        public:
                        VerifyUpdate(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 3) {}
                        VerifyUpdateOutput operator()(FunctionGroupList fg)
                        {
                            VerifyUpdateInput in;
                            in.FunctionGroups =fg ;
                            VerifyUpdateOutput out;
                            process_method_call<VerifyUpdateOutput, VerifyUpdateInput>(in, out);
                            return out;
                        }
                    };
                    class StopUpdateSession : public ara::com::proxy::method::MethodBase
                    {
                        public:
                        StopUpdateSession(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 4) {}
                        void operator()()
                        {
                         //fire and forget
                        }
                    };
                }

                class UpdateRequestproxy : public ara::com::proxy::ProxyBase
                {
                public:
                    UpdateRequestproxy(HandleType handle) : ProxyBase(handle), StartUpdateSession(handle.ptr2bindingProtocol) , PrepareUpdate(handle.ptr2bindingProtocol),VerifyUpdate(handle.ptr2bindingProtocol),StopUpdateSession(handle.ptr2bindingProtocol)
                    {
                    }
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService()
                    {
                        return ara::com::proxy::ProxyBase::FindService("/home/tabakh/Desktop/GP/src/OTA-Adaptive-AUTOSAR-Project/executables/ucm/etc/service_manifest.json",2);
                    }
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(ara::com::InstanceIdentifier InstanceID)
                    {
                        return ara::com::proxy::ProxyBase::FindService("/home/tabakh/Desktop/GP/src/OTA-Adaptive-AUTOSAR-Project/executables/ucm/etc/service_manifest.json",2, InstanceID);
                    }
                    methods::StartUpdateSession StartUpdateSession;
                    methods::PrepareUpdate PrepareUpdate;
                    methods::VerifyUpdate VerifyUpdate;
                    methods::StopUpdateSession StopUpdateSession;

                };
            }

        }
    }
}
