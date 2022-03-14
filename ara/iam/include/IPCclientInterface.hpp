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
            int Connect();
            void Send(std::string data, int clientSd);
            void sendPID(int clientPID);
            bool Receive(int clientSd);
        };

        std::string convertToString(char* a, int size);
    }
}