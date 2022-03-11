#pragma once
#include <iostream>

#include "../../core/types.hpp"

using namespace std;

namespace ara
{
    namespace com
    {
        enum class BindingProtocol
        {
            SOMEIP,
            DDS
        };
        class NetworkBindingBase
        {
        public:
            template <typename T, typename... args>
            ara::core::Future<T> SendRequest(uint16_t ID, args... Args);
            void OfferService(uint32_t,uint32_t,uint32_t);
            uint32_t receive(void);
            template <typename... args>
            void send(args... Args);
            void StopOfferService(uint32_t);
        };
        class SOMEIP : public NetworkBase
        {
            
        };
    }
}
