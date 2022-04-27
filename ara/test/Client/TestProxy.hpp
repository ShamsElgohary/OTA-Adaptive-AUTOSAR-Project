#pragma once
#include "serviceProxy.hpp"
#include "method.hpp"
#include "types.hpp"


namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            namespace proxy
            {
                struct AddInput
                {
                    uint64_t a;
                    uint64_t b;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &a;
                        ar &b;
                    }
                    friend class boost::serialization::access;
                };
                struct AddOutput
                {
                    uint64_t Sum;

                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &Sum;
                    }
                    friend class boost::serialization::access;
                };

                namespace methods
                {
                    class Add : public ara::com::proxy::method::MethodBase
                    {
                    public:
                        Add(std::shared_ptr<ara::com::NetworkBindingBase> h) : MethodBase(h, 1) {}
                        AddOutput operator()(uint64_t a, uint64_t b)
                        {
                            AddInput in;
                            in.a =a;
                            in.b =b;

                            AddOutput out;
                            process_method_call<AddOutput, AddInput>(in, out);
                            return out;
                        }
                    };

                }

                class PackageManagerProxy : public ara::com::proxy::ProxyBase
                {
                public:
                    PackageManagerProxy(HandleType handle) : ProxyBase(handle), Add(handle.ptr2bindingProtocol)
                    {
                    }
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService()
                    {
                        return ara::com::proxy::ProxyBase::FindService("/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/executables/com_proxy_test/etc/service_manifest.json",1);
                    }
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(ara::com::InstanceIdentifier InstanceID)
                    {
                        return ara::com::proxy::ProxyBase::FindService("/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/executables/com_proxy_test/etc/service_manifest.json",1, InstanceID);
                    }
                    methods::Add Add;
                };
            }
        }
    }
}
