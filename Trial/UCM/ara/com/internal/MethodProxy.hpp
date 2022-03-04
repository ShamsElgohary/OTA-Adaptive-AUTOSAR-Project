#ifndef _ARA_COM_INTERNAL_METHODPROXY_HPP_
#define  _ARA_COM_INTERNAL_METHODPROXY_HPP_

#include "Proxy.hpp"

namespace ara
{
    namespace com
    {
        namespace internal
        {
            template <typename Output>
            class MethodProxy {
                private: 
                    Proxy::HandleType Handle;
                    uint16_t M_ID;

                public:

                    MethodProxy(Proxy::HandleType& Handle, uint16_t M_ID)
                    {
                        this->Handle = Handle;
                        this->M_ID = M_ID;
                    }
                
                    template <typename... args>
                    ara::core::Future<Output> operator()(args... Args)
                    {
                        return this->Handle.network_binding.SendRequest(M_ID, args);
                    }
            };


            class MethodOneWayProxy {
                private: 
                    Proxy::HandleType Handle;
                    uint16_t M_ID;

                public:

                    MethodOneWayProxy(Proxy::HandleType& Handle, uint16_t M_ID)
                    {
                        this->Handle = Handle;
                        this->M_ID = M_ID;
                    }
                
                    template <typename... args>
                    void operator()(args... Args)
                    {
                        this->Handle.network_binding.SendRequest(M_ID, args);
                    }
            };
        }
    }
}

#endif