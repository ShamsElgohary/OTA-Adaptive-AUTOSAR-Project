#pragma once

#include "../../com/language_binding/proxy/include/serviceProxy.hpp"
#include "../../com/language_binding/proxy/include/method.hpp"
#include "../../com/include/types.hpp"

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            namespace proxy
            {
                /** Add **/
                struct AddInput
                {
                    uint64_t a;
                    uint64_t b;
                };
                ///////////////////////////// PROBLEM //////////////////////////////////
                struct AddOutput
                {
                    // Size (in bytes) of the Software Package to be transferred
                    uint64_t Sum; // Size of the blocks to be received with TransferData method // Success or Failure
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
                            this->Delegate->SendRequest(this->ID, a, b);
                            AddOutput in;
                            this->Delegate->ReceiveMessage(in);
                            return in;
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
                        ara::com::proxy::ProxyBase::FindService(1);
                    }

                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(ara::com::InstanceIdentifier InstanceID)
                    {
                        ara::com::proxy::ProxyBase::FindService(1, InstanceID);
                    }

                    methods::Add Add;
                };
            }
        }
    }
}
