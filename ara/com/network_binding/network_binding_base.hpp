#pragma once 
#include <iostream>
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
