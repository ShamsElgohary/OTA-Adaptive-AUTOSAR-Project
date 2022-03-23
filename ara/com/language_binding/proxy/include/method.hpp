#pragma once
#include <iostream>
#include "serviceProxy.hpp"
using namespace std;
namespace ara
{
    namespace com
    {
        namespace proxy
        {
            namespace method
            {   
                
                class MethodBase
                {
                public:
                    std::shared_ptr<ara::com::NetworkBindingBase> Delegate;
                    uint16_t ID;
                    MethodBase(std::shared_ptr<ara::com::NetworkBindingBase>ptr, uint16_t id);
                    template <typename output, typename... Params>
                    output operator()(Params... args);
                };
            }
        }
    }
}
