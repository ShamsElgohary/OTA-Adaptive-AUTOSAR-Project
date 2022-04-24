#pragma once

#include "../../sm/types.hpp"
#include "../com/language_binding/proxy/include/serviceProxy.hpp"
#include "../com/language_binding/proxy/include/method.hpp"
namespace ara
{
    namespace sm
    {
        namespace UpdateRequest
        {
            namespace proxy
            {
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

                namespace methods
                {
                    class StartUpdateSession : public ara::com::proxy::method::MethodBase
                    {
                        public:
                        StartUpdateSession(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 1) {}
                        StartUpdateSessionOutput operator()(uint8_t AppError)
                        {
                            StartUpdateSessionOutput out;
                            process_method_call<StartUpdateSessionOutput>(out);
                            return out;
                        }
                    };

                    class PrepareUpdate : public ara::com::proxy::method::MethodBase
                    {
                        public:
                        PrepareUpdate(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 1) {}
                        AddOutput operator()()
                        {
                            AddInput in;
                            in.x =x ;
                            AddOutput out;
                            process_method_call<AddOutput, AddInput>(in, out);
                            return out;
                        }
                    };

                    class VerifyUpdate : public ara::com::proxy::method::MethodBase
                    {
                        public:
                        Add(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 1) {}
                        AddOutput operator()(vector<s> x)
                        {
                            AddInput in;
                            in.x =x ;
                            AddOutput out;
                            process_method_call<AddOutput, AddInput>(in, out);
                            return out;
                        }
                    };
                    class StopUpdateSession : public ara::com::proxy::method::MethodBase
                    {
                        public:
                        Add(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 1) {}
                        AddOutput operator()(vector<s> x)
                        {
                            AddInput in;
                            in.x =x ;
                            AddOutput out;
                            process_method_call<AddOutput, AddInput>(in, out);
                            return out;
                        }
                    };
                }

                class UpdateRequestproxy : public ara::com::proxy::ProxyBase
                {
                public:
                    UpdateRequestproxy(HandleType handle) : ProxyBase(handle), StartUpdateSession(handle.ptr2bindingProtocol)
                    {
                    }
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService()
                    {
                        return ara::com::proxy::ProxyBase::FindService(1);
                    }
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(ara::com::InstanceIdentifier InstanceID)
                    {
                        return ara::com::proxy::ProxyBase::FindService(1, InstanceID);
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
