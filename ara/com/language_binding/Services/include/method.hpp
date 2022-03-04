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
                class MethodBase{
                    public:
                    int id ;
                    ServiceProxy::HandleType handle;
                    MethodBase(ServiceProxy::HandleType&h) 
                    {
                        handle = h ;
                    }
                    // operator overloading for regular functions
                    template<type T ,typename... Args>
                    future<T> operator()(Args&&... args)
                    {
                        return handle.network_binding.SendRequest(args) ;
                    }
                    // operator overloading for fire and forget functions 
                    template<typename... Args>
                    void operator()(Args&&... args)
                    {
                        return handle.network_binding.SendRequest(args) ;
                    }
                }
               class calibrate : public MethodBase
               {
                   //use base class constructor
                   calibrate(handle h):MethodBase(h){};
               };
               class adjust    : public MethodBase{}
               {
                   //use base class constructor
                   adjust(handle h):MethodBase(h){};
               }
            }
        }
    }
}
