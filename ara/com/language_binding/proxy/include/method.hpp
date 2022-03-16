#pragma once
#include <iostream>
#include "serviceProxy.hpp"

using namespace std;

namespace ara
{
    namespace com
    {
        namespace proxy
        {
            namespace method
            {
                struct Output{
                    bool success;
                };

                class MethodBase
                {
                public:
                    ara::com::proxy::HandleType *Delegate;
                    //ara::com::NetworkBindingBase *networkBindingPtr;
                    std::string name;
                    uint32_t M_ID;
                    Output op;
                    /* IMPLEMENTATION IN GENERATION */
                    MethodBase(ara::com::NetworkBindingBase *ptr);
                    template <typename...Params>
                    ara::core::Future<Output> operator()(Params...args)
                };
                class MethodOneWayBase
                {
                protected:
                    //ara::com::proxy::HandleType Handle;
                    ara::com::NetworkBindingBase *networkBindingPtr;
                    std::uint16_t ID;

                public:
                    /* IMPLEMENTATION IN GENERATION */
                    MethodOneWayBase(ara::com::NetworkBindingBase *ptr);

                    /* SHOULD (This isnot the Actual Implementation) */
                    template <typename... args>
                    void operator()(int x)
                    {
                        this->Handle.network_binding->SendRequestFireAndForget(ID, x);
                    }
                };
            }
        }
    }
}

class ucmproxy
{
    method::starttarnsfer starttransfer;
}

proxy x(handle1);
x.starttarnsfer();

namespace method
{
    class starttarnsfer : public MethodBase
    {
        method_id = 5;
    }
}
