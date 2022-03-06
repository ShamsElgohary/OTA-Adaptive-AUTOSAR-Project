#pragma once 
#include <iostream>
#include "../../../language_binding/Services/include/serviceProxy.hpp"
using namespace std ;

namespace ara 
{
    namespace com
    {
        namespace proxy 
        {
            namespace methods
            {
                template <typename Output>
                class MethodBase {
                    private: 
                        ProxyBase::HandleType Handle;
                        uint16_t M_ID;
                    public:
                        MethodBase(ProxyBase::HandleType& Handle, uint16_t M_ID)
                        {
                            this->Handle = Handle;
                            this->M_ID = M_ID;
                        }
                        template <typename... args>
                        ara::core::Future<Output> operator()(args... Args)
                        {
                            return this-> Handle.network_binding -> SendRequest(M_ID, args);
                        }
                };
                class MethodOneWayBase {
                    private: 
                        ProxyBase::HandleType Handle;
                        uint16_t M_ID;

                    public:

                        MethodOneWayBase(ProxyBase::HandleType& Handle, uint16_t M_ID)
                        {
                            this->Handle = Handle;
                            this->M_ID = M_ID;
                        }
                    
                        template <typename... args>
                        void operator()(args... Args)
                        {
                            this->Handle.network_binding -> SendRequestFireAndForget(M_ID, args);
                        }
                };
            //    class calibrate : public MethodBase
            //    {
            //        //use base class constructor
            //        calibrate(handle h):MethodBase(h){};
            //    };
            //    class adjust    : public MethodBase{}
            //    {
            //        //use base class constructor
            //        adjust(handle h):MethodBase(h){};
            //    };
            }
        }
    }
}
