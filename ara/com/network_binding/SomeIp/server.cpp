#include "someip.hpp"

using namespace boost::asio;
using namespace std;
using namespace someip;

int main() {
      
    /// SETTING UP SERVER ////
    uint16_t port = 1234;
    SomeIpConfiguration someipConfig{TransportProtocol::TCP,EndUserType::SERVER};
    boost::asio::io_service io_service;
    shared_ptr<someipConnection> serverUser = someipConnection::SetSomeIpConfiguration(io_service, port , someipConfig);
    std::stringstream ss;


    // KEEP WAITING FOR CONNECTIONS
    serverUser->ServerListen();
    ss << serverUser->ReceiveMessage().payload;
    cout << ss.str() << endl;
    //serverUser->CloseConnection();

    ss.str(""); // EMPTY STRINGSTREAM

    //// MESSAGE RESPONSE SETUP ////
    std::stringstream data;
    
    Serializer S;
    
    S.Serialize(data,1000,1000,std::string("MY NAME IS SHAMS"),std::string(" RESPONSE TEST SUCCESS"));    

    someipHeader s;     // SET SOMEIP HEADER (THIS IS ONLY FOR EXAMPLE) ///

    someip_Message msg(s,data);

    serverUser->SendResponse(msg);

    return 0;
}
