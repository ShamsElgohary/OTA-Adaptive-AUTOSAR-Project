#pragma once 
#include <iostream>
#include "serviceProxy.hpp"

using namespace std ;

namespace ara 
{
    namespace com
    {
        namespace proxy 
        {
            namespace method
            {
                template <typename Output>
                virtual class MethodBase {
                    protected: 
                        ara::com::proxy::ProxyBase::HandleType Handle;
                        std::string name;
                        std::uint_16 D;
                    public:
                        /* IMPLEMENTATION IN GENERATION */
                        MethodBase(ara::com::proxy::ProxyBase::HandleType& Handle);

                        /* MUST (This isnot the Actual Implementation) */
                        template <typename... args>
                        ara::core::Future<Output> operator()(args... Args)
                        {
                            return this-> Handle.network_binding -> SendRequest(M_ID, args);
                        }
                };
                class MethodOneWayBase {
                    protected: 
                        ara::com::proxy::ProxyBase::HandleType Handle;
                        std::uint16_t ID;

                    public:
                        /* IMPLEMENTATION IN GENERATION */
                        MethodOneWayBase(ara::com::proxy::ProxyBase::HandleType& Handle);

                        /* SHOULD (This isnot the Actual Implementation) */
                        template <typename... args>
                        void operator()(int x)
                        {
                            this->Handle.network_binding -> SendRequestFireAndForget(ID, x);
                        }
                };
            }
        }
    }
}


class ucmproxy
{
    method::starttarnsfer starttransfer ;
}

proxy x(handle1 ) ;
x.starttarnsfer() ;


namespace method 
{
    class starttarnsfer ;public MethodBase
    {
        method_id = 5;
    }
}
