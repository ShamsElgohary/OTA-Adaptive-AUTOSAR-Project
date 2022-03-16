#pragma once

#include "../com/language_binding/proxy/include/serviceProxy.hpp"
#include "../com/language_binding/proxy/include/method.hpp"
#include "../com/language_binding/proxy/include/field.hpp"
#include "../com/language_binding/proxy/include/event.hpp"

namespace Name1
{
    namespace Name2
    {
        namespace Proxy
        {
            namespace methods
            {
                class DoMethod : public ara::com::proxy::method::MethodBase
                {
                    // HOW TO REPRESENT OUTPUT TYPES?
                    public:
                    DoMethod(HandleType *ptr) : ara::com::proxy::method::MethodBase(ptr, "TransferStart")
                    {}
                }
            }

            class AppProxy : public ara::com::proxy::ProxyBase
            {
                public:

                methods::DoMethod DoMethod(ProxyHandle);

                AppProxy(HandleType handle) : ara::com::proxy::ProxyBase(handle, 1000)
                {
                    if (handle.InstanceID == 1) // SOMEIP MAPPING FOR EXAMPLE
                    {
                        DoMethod.SetID(100);
                    }
                    else if (handle.InstanceID == 2) // DDS MAPPING FOR EXAMPLE
                    {
                        DoMethod.SetID(101);
                    }
                }
            }
        }
    }
}