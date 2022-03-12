#pragma once
#include "types.h"

namespace ara
{
    namespace iam
    {
        class IPCclientInterface
        {
        private:
            int socket_descriptor;

        public:
            void connect();
            void send(std::stringstream data);
            bool receive();
        };
    }
}