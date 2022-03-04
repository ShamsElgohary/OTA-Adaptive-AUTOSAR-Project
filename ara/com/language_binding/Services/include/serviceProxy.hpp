#pragma once 
#include <iostream>
#include <memory>
#include "../../../network_binding/network_binding_base.hpp"
#include "../../../include/types.hpp"
using namespace std ;

namespace ara 
{
    namespace com
    {
        namespace proxy 
        {
            class ServiceProxy
            {
            public:
                class handle
                {
                    public:
                    shared_ptr<NetworkBase> network_binding ;
                    InstanceIdentifier id ;
                    handle();
                };
                handle h ;
                method::calibrate ;
                ServiceProxy(handle){
                    calibrate(handle) ;
                }
                MethodBase calibrate;
            };
        }
    }
}


ServiceProxy.calibrate() 