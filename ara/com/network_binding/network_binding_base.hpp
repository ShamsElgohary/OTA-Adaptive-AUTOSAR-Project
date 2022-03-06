#pragma once 
#include <iostream>

#include "../../core/types.hpp"

using namespace std ;

namespace ara 
{
    namespace com
    {
        class NetworkBase
        {
            public:
                template <typename T, typename... args>
                ara::core::Future <T> SendRequest(uint16_t ID, args... Args);
        };
        
    }
}
