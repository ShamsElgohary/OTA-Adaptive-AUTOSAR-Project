#pragma once
#include "../lib/include/types.hpp"
#include "../lib/include/Grant.hpp"
#include "../lib/include/ipc_parameters.h"

namespace ara
{
    namespace iam
    {
        class IPCserverInterface
        {
        private:
            int portnum = IAM_PORT_NUMBER;
            std::string IP = IAM_IP_ADDRESS;
            sockaddr_in servAddr;
            int ServerSD;
            int clientPID;


        public:
            IPCserverInterface() = default;
            ~IPCserverInterface() = default;
            std::uint8_t ServerSocketInit();
            int getPeerId(int sd);
            int Listen();
            void Send(bool is_granted, int sd);
            ara::iam::Grant Receive(int sd);
        };
    }
}