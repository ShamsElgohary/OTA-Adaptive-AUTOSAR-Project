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
                template <typename... Params ,typename output>
                output MethodBase::operator()(Params... args)
                {
                    ara::com::proxy::method::MethodBase::Delegate->ptr2bindingProtocol->SendRequest(this->ID, args...);
                    output in ;
                    ara::com::proxy::method::MethodBase::Delegate->ptr2bindingProtocol->receive(in);
                    return in;
                }
            }
        }
    }
}
