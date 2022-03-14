#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#define EM_IP_ADDRESS "127.0.0.1"
#define EM_PORT_NUMBER 8080
#define BUFF_SIZE 40

using namespace std ;

namespace ara
{
    namespace em
    {
        class FindProcessClient
        {
        private:
            int portNum = EM_PORT_NUMBER;
            string IP = EM_IP_ADDRESS;
            int server_socket ;
            char buff[BUFF_SIZE];
        public:
            FindProcessClient();
            ~FindProcessClient();
            void sendData(int pid);
            string receiveData();
        };
    }
}