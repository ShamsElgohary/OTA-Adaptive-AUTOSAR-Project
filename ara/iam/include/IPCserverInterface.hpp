#pragma once
#include "types.h"

namespace ara
{
    namespace iam
    {
        class IPCserverInterface
        {
        public:
            int getPeerId(int clientSocketDecriptor);
            void connect();
            void send(std::stringstream data);
            ara::iam::Grant receive();
        };
    }
}