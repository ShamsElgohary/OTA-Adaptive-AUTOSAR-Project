
#include "servicediscovery.cpp"
#include"../include/servicestorage.hpp"
#include "someip_sd_message.cpp"
#include "string.h"

uint16_t service_id;
uint16_t instance_id;
uint16_t TTL;
uint8_t type;
uint32_t ipv4_address;
uint8_t port_num;

int main()
{    
   while (1)
   {
    std::string flag;
    std::stringstream ss;
    uint16_t ttl;
    someip::someip_Message msg1;
    //msg1=someip::ReadsomeipMessage(boost::asio::ip::tcp::socket & socket);  //read

    SomeIpConfiguration someipConfig{TransportProtocol::TCP,EndUserType::SERVER};

    boost::asio::io_service io_service;

    shared_ptr<someipEndUser> serverUser = someipEndUser::SetSomeIpConfiguration(io_service, MULTICAST_PORT , someipConfig);

    serverUser->ServerListen();

    ss << serverUser->ReceiveMessage().payload;

    cout << ss.str()<<endl;
    



}}

