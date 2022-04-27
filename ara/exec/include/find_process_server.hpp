#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <boost/asio.hpp>



using namespace boost::asio;
using ip::tcp;
using namespace std;

#define EM_PORT_NUMBER 8084
#define EM_IP_ADDRESS "127.0.0.1"



namespace ara
{
    namespace exec
    {
        class FindProcessServer
        {
        private:
            int pid;
            boost::asio::io_service io_service;
            tcp::acceptor acceptor_;
            tcp::socket socket_;

        public:
            FindProcessServer();
            ~FindProcessServer();
            int receiveData();
            void sendData(string processName);
        };
    }
}