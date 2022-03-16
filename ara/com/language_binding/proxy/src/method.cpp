#pragma once
#include "../include/method.hpp"
#include <iostream>
using namespace std;

namespace ara
{
    namespace com
    {
        namespace proxy
        {
            namespace method
            {
                class MethodBase
                {
                public:
                    MethodBase(ara::com::NetworkBindingBase *ptr, const std::string& N) : Delegate(ptr), name(N)
                    {
                        //ara::com::proxy::method::MethodBase::Delegate = ara::com::proxy::HandleTypeFactory::create(Handle.GetBindingProtocol());
                        //ara::com::proxy::method::MethodBase::Delegate = ptr;
                    }

                    SetID(const std::uint32_t& id)
                    {
                        ID = id;
                    }

                    template <typename...Params>
                    std::future<ara::com::proxy::method::Output> operator()(Params...args) //variadic function
                    {
                        //return this->Handle.network_binding->SendRequest(M_ID, arg);
                        std::promise<Output> p;
                        auto f = p.get_future();
                        Output op = ara::com::proxy::method::MethodBase::Delegate->networkBindingPtr->SendRequest(ara::com::proxy::method::MethodBase::M_ID , std::forward<Params>(args)...);
                        p.set_value(op);
                        return f;
                        //return this->Delegate.SendRequest(M_ID, arg);
                    }
                }

            }
        }
    }
}
