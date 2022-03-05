#pragma once 
#include <iostream>
#include <memory>
#include "../../../network_binding/network_binding_base.hpp"
#include "../../../include/types.hpp"
#include "../../../language_binding/Services/include/method.hpp"
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
                class HandleType
                {
                    public:
                    handle();
                    shared_ptr<NetworkBase> network_binding ;
                    InstanceIdentifier id ;
                };
                //methods
                methods::calibrate Calibrate;
                methods::adjust Adjust;
                //constructor
                //each function takes handle in each constructor
                explicit ServiceProxy(HandleType&handle):Calibrate(handle),Adjust(handle);
                
            };
        }
    }
}
