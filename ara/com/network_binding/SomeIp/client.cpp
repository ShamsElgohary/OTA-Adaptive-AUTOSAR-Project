#include "someip.hpp"

using namespace boost::asio;
using namespace std;
using namespace someip;


int main() {

    /// MESSAGE SETUP ////

    int z = 10;

    std::stringstream data;
    
    Serializer S;
    
    S.Serialize(data,z,10,std::string("MY NAME IS SHAMS"),std::string("TEST SUCCESS"));

    someipHeader s;     // SET SOMEIP HEADER (THIS IS ONLY FOR EXAMPLE) ///

    someip_Message msg(s,data);



    /// SETTING UP CLIENT ////

    uint16_t port = 1234;

    SomeIpConfiguration someipConfig{TransportProtocol::TCP,EndUserType::CLIENT};

    boost::asio::io_service io_service;

    shared_ptr<someipConnection> clientUser = someipConnection::SetSomeIpConfiguration(io_service, port , someipConfig);
    
   // clientUser->SendMessage(msg);

    // RECEIVE RESPONSE //

    data.str("");

   // msg.header.setMessageType( MessageType::REQUEST);

    msg = clientUser->SendRequest(msg);

    data << msg.payload;

    cout << data.str();

    //clientUser->CloseConnection();

    cout << "CLIENT FINISHED \n";

    return 0;
}


    // CALLING ClientTCP directly

    //ClientTCP clientInstance(io_service, 1234);

    //ClientInstance.ProxyConnect(); 
    
    //clientInstance.SendMessage(msg);    
