#pragma once
#include "../include/method.hpp"
#include <iostream>
#include <future>
using namespace std;

namespace ara
{
    namespace com
    {
        namespace proxy
        {
            namespace method
            {
                /*Constructor to be generated*/

                template <typename Output, typename... Params>
                future<Output> MethodBase::operator()(Params... args)
                {
                    std::promise<Output> p;
                    auto f = p.get_future();
                    Output op = ara::com::proxy::method::MethodBase::Delegate->ptr2bindingProtocol->SendRequest(ara::com::proxy::method::MethodBase::ID, args...);
                    p.set_value(op);
                    return f;
                }

            }
        }
    }
}