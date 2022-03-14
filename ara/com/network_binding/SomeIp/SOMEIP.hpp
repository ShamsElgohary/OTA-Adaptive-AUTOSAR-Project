#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <functional>

#include "SOMEIP_Message.hpp"

namespace someip {
					
static const std::string LOOPBACK_IP = "127.0.0.1";
static const std::string MULTICAST_IP = "239.255.0.1";	// mulicast address to use. see: http://en.wikipedia.org/wiki/Multicast_address
static const int MULTICAST_PORT = 5000;					// Service Discovery PORT

static const boost::asio::ip::address& localIp = boost::asio::ip::address::from_string(LOOPBACK_IP);


enum TransportProtocol{
	TCP,UDP
};

enum EndUserType{
	CLIENT,SERVER
};


typedef struct
{
	TransportProtocol tpType = TransportProtocol::TCP; 
	EndUserType endUserType;
}SomeIpConfiguration;



    /////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP CLIENT  	/////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


class someipEndUser{
	
	protected: 

		InstanceID instance;
		TransportProtocol tpType;
		EndUserType endUserType;
	
	public:

		someipEndUser(SomeIpConfiguration someipConfig);

		/* DEFAULT CONSTRUCTOR */
		someipEndUser();

		/* DEFAULT DESTRUCTOR */
		~someipEndUser();


		//CONFGIURE SOMEIP TO SUIT USER : 
		//TCP OR UDP MESSAGES?  
		//CLIENT OR SERVER? 
		//SYNCHRONOUS OR ASYNCHRONOUS 
		// SERVICE PORT
		// io_service TO USE
		
		static std::shared_ptr<someipEndUser> SetSomeIpConfiguration(
			boost::asio::io_service& io_service, 
			uint16_t port, 	 
			SomeIpConfiguration someipConfig,					// DEFAULT VALUES INSIDE
			std::string IPv4 = LOOPBACK_IP	 					// DEFAULT
		);


		/* TCP OR UDP MESSAGES? */
		TransportProtocol GetTransportProtocolType();

		/* END USER TYPE? */
		EndUserType GetEndUserType();

		/* SERVER LISTENING */
		virtual void ServerListen();

		/* CONNECT PROXY TO SERVER */
		virtual bool ProxyConnect();	

		/* OPEN SOCKET CONNECTION*/
		virtual bool OpenConnection() = 0;

		/* CLOSE SOCKET CONNECTION */
		virtual bool CloseConnection() = 0;

		/* FUNCTION TO SEND A SOMEIP MESSAGE USING TCP */
		virtual bool SendMessage(someip_Message &msg ) = 0;

		/* FUNCTION TO READ A SOMEIP MESSAGE USING TCP */
		virtual someip_Message ReadMessage() = 0;				


};


	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP UDP  ////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

class someipUDP  : public someipEndUser  {
	
	private:

	boost::asio::ip::udp::endpoint endpoint;
	boost::asio::ip::udp::socket socket_;
	boost::asio::deadline_timer timer;
	uint16_t port;

	public:

	/* CONSTRUCTOR */
	someipUDP(boost::asio::io_service& io_service, uint16_t port); 	
	
	/* FUNCTION TO SEND A SOMEIP MESSAGE USING UDP */
	void SendMessage(someip_Message &msg, uint16_t port);

	/* FUNCTION TO READ A SOMEIP MESSAGE USING UDP */
	someip_Message ReadMessage();



};


} // End of Namespace someip

