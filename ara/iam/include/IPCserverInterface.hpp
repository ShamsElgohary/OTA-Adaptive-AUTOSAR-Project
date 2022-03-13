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