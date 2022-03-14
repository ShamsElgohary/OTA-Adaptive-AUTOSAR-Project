#include "SOMEIP.hpp"
#include "SOMEIP_TCP.hpp"

using namespace boost::asio;
using namespace std;
using namespace someip;


int main() {
      
    /// SETTING UP SERVER ////

    uint16_t port = 1234;

    SomeIpConfiguration someipConfig{TransportProtocol::TCP,EndUserType::SERVER};

    boost::asio::io_service io_service;

    shared_ptr<someipEndUser> serverUser = someipEndUser::SetSomeIpConfiguration(io_service, port , someipConfig);

    std::stringstream ss;

    serverUser->ServerListen();

    ss << serverUser->ReadMessage().payload;

    cout << ss.str();

    return 0;
}
