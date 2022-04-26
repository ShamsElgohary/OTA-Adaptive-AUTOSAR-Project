#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#define EM_PORT_NUMBER 8080
#define EM_IP_ADDRESS "127.0.0.1"
using namespace std;

namespace ara
{
    namespace exec
    {
        class FindProcessServer
        {
        private:
            int portNum = EM_PORT_NUMBER;
            string IP = EM_IP_ADDRESS;
            int pid; 
            int server_socket ;
            int client_socket ;
         
        public:
            FindProcessServer();
            ~FindProcessServer();
            int receiveData();
            void connect();
            void disconnect();
            void sendData(string processName);
        };
    }
}