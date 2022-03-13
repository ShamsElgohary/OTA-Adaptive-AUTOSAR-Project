#pragma once
#include "types.hpp"

namespace ara
{
    namespace iam
    {
        class IPCclientInterface
        {
        private:
            int portnum = 2000;
            std::string IP = "127.0.0.1";

        public:
            void connect();
            void send(std::stringstream data);
            bool receive();
        };
    }
}