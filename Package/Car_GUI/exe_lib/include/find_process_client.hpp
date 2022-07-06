#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <boost/asio.hpp>

#define EM_IP_ADDRESS "127.0.0.1"
#define EM_PORT_NUMBER 8080

using namespace boost::asio;
using ip::tcp;
using namespace std ;
namespace ara
{
    namespace exec
    {
        class FindProcessClient
        {
        private:
        boost::asio::io_service io_service;
        tcp::socket socket_;
        public:
            FindProcessClient();
            ~FindProcessClient();
            void sendData(int pid);
            string receiveData();
        };
    }
}