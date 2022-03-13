#pragma once
#include "types.hpp"
#include "Grant.hpp"

namespace ara
{
    namespace iam
    {
        class IPCserverInterface
        {
        private:
            int portnum = 2000;
            std::string IP = "127.0.0.1";

        public:
            IPCserverInterface();
            ~IPCserverInterface();
            int getPeerId(int clientSocketDecriptor);
            void connect();
            void send(std::stringstream data);
            ara::iam::Grant receive();
        };
    }
}