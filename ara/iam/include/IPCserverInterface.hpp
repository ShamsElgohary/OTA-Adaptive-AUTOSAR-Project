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
            boost::asio::io_service io_service;
            tcp::acceptor acceptor_;
        public:
            tcp::socket socket_;
            IPCserverInterface() ;
            ~IPCserverInterface() ;
            std::uint8_t ServerSocketInit();
            int Listen();
            void Send(bool is_granted, int sd);
            ara::iam::Grant Receive(int sd);
        };
    }
}