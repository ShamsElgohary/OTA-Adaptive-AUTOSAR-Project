#pragma once
#include "serviceProxy.hpp"
#include "method.hpp"
#include "types.hpp"

struct s
{
    int data;

private:
    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &data;
    }
    friend class boost::serialization::access;
};
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
                    vector<s> x;
                private:
                    template <typename Archive>
                    void serialize(Archive &ar, const unsigned int version)
                    {
                        ar &x;
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

                class PackageManagerProxy : public ara::com::proxy::ProxyBase
                {
                public:
                    PackageManagerProxy(HandleType handle) : ProxyBase(handle), Add(handle.ptr2bindingProtocol)
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
                    methods::Add Add;
                };
            }
        }
    }
}
