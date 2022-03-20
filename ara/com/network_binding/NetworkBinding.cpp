#include <iostream>
#include <memory>
#include <string>
#include "NetworkBinding.hpp"
#include "types.hpp"

using namespace std;

namespace ara
{
    namespace com
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// SOMEIP NETWORK BINDING ///////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        SomeIpNetworkBinding::SomeIpNetworkBinding(int port, string ip)
        {
            this->port = port;
            this->ip = ip;
        }
        template <typename... Params>
        void SomeIpNetworkBinding::send(Params... args)
        {
        }
        template <typename T>
        void SomeIpNetworkBinding::receive(T &in)
        {
        }
        template <typename... Params>
        void SomeIpNetworkBinding::SendRequest(uint32_t methodID, Params... args)
        {
        }
    }
}
