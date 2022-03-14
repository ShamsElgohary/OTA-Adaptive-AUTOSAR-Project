#pragma once
#include "types.hpp"
#include "Grant.hpp"
#include "ipc_parameters.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define EM_IP_ADDRESS "127.0.0.1"
#define EM_PORT_NUMBER 8080

namespace ara
{
    namespace em
    {
        class FindProcessServer
        {
        private:
            int portNum = EM_PORT_NUMBER;
            int pid; 
            int server_socket ;
            int client_socket ;
        public:
            FindProcessServer();
            ~FindProcessServer();
            int receiveData();
            void sendData(string processName);
        };
    }
}