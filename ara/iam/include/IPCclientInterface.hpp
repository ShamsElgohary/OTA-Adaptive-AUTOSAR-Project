#pragma once
#include "types.hpp"
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

        public:
            void connect();
            void send(std::stringstream data);
            bool receive();
        };
    }
}