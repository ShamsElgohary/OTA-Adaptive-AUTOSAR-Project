#pragma once
#include "types.hpp"
#include "Grant.hpp"
#include "ipc_parameters.h"

namespace ara
{
    namespace iam
    {
        class IPCclientInterface
        {
        private:
            int portnum = IAM_PORT_NUMBER;
            std::string IP = IAM_IP_ADDRESS;
            int clientSd;
            bool is_granted;

        public:
            std::uint8_t Connect();
            void Send(ara::iam::Grant G);
            void sendPID(int pid);
            bool Receive();
        };
    }
}