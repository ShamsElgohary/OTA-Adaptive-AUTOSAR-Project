#pragma once

#include "../../com/language_binding/proxy/include/serviceProxy.hpp"
#include "../../com/language_binding/proxy/include/method.hpp"
#include "../../com/include/types.hpp"
#include "serialization_simple.hpp"

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
                            stringstream payload;
                            Serializer2 S;
                            cout << "1" << endl;
                            S.serialize(payload, a, b);
                            cout << payload.str() << endl;
                            this->Delegate->SendRequest(this->ID, payload);
                            AddOutput out;
                            cout << "3" << endl;
                            stringstream out_ss = this->Delegate->ReceiveMessage();
                            cout << "4" << endl;
                            Deserializer2 D;
                            D.deserialize(out_ss, out);
                            cout << "5" << endl;

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
