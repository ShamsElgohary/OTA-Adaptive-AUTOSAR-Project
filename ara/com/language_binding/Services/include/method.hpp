#pragma once 
#include <iostream>
using namespace std ;

namespace ara 
{
    namespace com
    {
        namespace proxy 
        {
            namespace method
            {
                class MethodBase{
                    public:
                    int id ;
                    MethodBase(ServiceProxy::handle h) 
                    {
                        h = h ;
                    }
                    ServiceProxy::handle h ;
                    template<type T ,typename... Args>
                    future<T> operator(Args&&... args)
                    {
                        return h.network_binding.SendRequest(args) ;
                    }
                    template<typename... Args>
                    void operator(Args&&... args)
                    {
                        return h.network_binding.SendRequest(args) ;
                    }
                }
                class calibrate : public MethodBase{};
            }
        }
    }
}
