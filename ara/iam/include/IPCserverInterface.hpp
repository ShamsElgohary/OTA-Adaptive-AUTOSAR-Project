#pragma once
#include "types.hpp"
#include "Grant.hpp"
#include "ipc_parameters.h"

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


        public:
            IPCserverInterface() = default;
            ~IPCserverInterface() = default;
            std::uint8_t ServerSocketInit();
            // int getPeerId(int clientSocketDecriptor);
            int Listen();
            void Send(bool is_granted, int sd);
            ara::iam::Grant Receive(int sd);
        };
    }
}