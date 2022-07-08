#pragma once
#include "types.hpp"
#include "Grant.hpp"
#include "ipc_parameters.h"

namespace ara
{
    namespace iam
    {
        class IPCclientInterface
        {
        private:
            boost::asio::io_service io_service;
            tcp::socket socket_;

        public:
            IPCclientInterface();
            ~IPCclientInterface();
            std::uint8_t Connect();
            void Send(ara::iam::Grant G);
            bool Receive();
        };
    }
}