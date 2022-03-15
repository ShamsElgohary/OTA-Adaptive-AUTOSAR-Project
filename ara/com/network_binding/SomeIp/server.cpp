#include "someip.hpp"


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


    while(1)
        {
            // KEEP WAITING FOR CONNECTIONS
        serverUser->ServerListen();
        ss << serverUser->ReceiveMessageAsynch().payload;
        // FOR ASYNCHRONOUS
        //io_service.run();
        cout << ss.str() << endl;
        serverUser->CloseConnection();
        ss.str(""); // EMPTY STRINGSTREAM
        }

    return 0;
}
