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
                class MethodBase
                {
                public:
                    ProxyBase::HandleType *Delegate;
                    std::string name;
                    uint32_t ID;
                    MethodBase(ara::com::NetworkBindingBase *ptr, const std::string &N);
                    template <typename Output, typename... Params>
                    future<Output> operator()(Params... args);
                };
            }
        }
    }
}
