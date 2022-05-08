#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <functional>
#include "someipMessage.hpp"

const uint32_t SOMEIP_BUFFER_SIZE = 4096;

namespace someip {
					
static const std::string LOOPBACK_IP = "127.0.0.1";		
static const std::string MULTICAST_IP = "239.255.0.1";	// mulicast address to use. see: http://en.wikipedia.org/wiki/Multicast_address
static const int MULTICAST_PORT = 3000;					// Service Discovery PORT

static const boost::asio::ip::address& localIp = boost::asio::ip::address::from_string(LOOPBACK_IP);


enum TransportProtocol{
	TCP,UDP
};

enum EndUserType{
	CLIENT,SERVER
};

enum SecurityType{
	RAW,TLS,IPSec
};


typedef struct
{
	TransportProtocol tpType; 
	EndUserType endUserType;
	SecurityType securityType;
}SomeIpConfiguration;


	////////////////////////////// HANDLES FOR ASYNCH OPERATIONS ///////////////////////////////////////

		/* USED AS A HANDLE IN THE SOCKET FOR ASYNCHRONOUS MESSAGES */
	void OnSendCompleted(boost::system::error_code ec, size_t bytes_transferred);

			/* USED AS A HANDLE IN THE SOCKET FOR ASYNCHRONOUS MESSAGES */
	void OnReceiveCompleted(boost::system::error_code ec, size_t bytes_transferred);


	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP CONNECTION    ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

class someipConnection{
	
	protected: 

		InstanceID instance;
		TransportProtocol tpType;
		EndUserType endUserType;
	
	public:

		someipConnection(SomeIpConfiguration someipConfig);

		/* DEFAULT CONSTRUCTOR */
		someipConnection();

		/* DEFAULT DESTRUCTOR */
		~someipConnection();


		//CONFGIURE SOMEIP TO SUIT USER : 
		//TCP OR UDP MESSAGES?  
		//CLIENT OR SERVER? 
		//SYNCHRONOUS OR ASYNCHRONOUS 
		// SERVICE PORT
		// io_service TO USE
		
		static std::shared_ptr<someipConnection> SetSomeIpConfiguration(
			boost::asio::io_context & io_service, 
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

		/* CLOSE SOCKET CONNECTION */
		virtual bool CloseConnection() = 0;

		/* FUNCTION TO SEND A SOMEIP MESSAGE USING TCP */
		virtual bool SendMessage(someipMessage &msg) = 0;

		/* FUNCTION TO READ A SOMEIP MESSAGE USING TCP */
		virtual someipMessage ReceiveMessage() = 0;		

		/* FUNCTION TO SEND A SOMEIP MESSAGE ASYNCH */
		virtual bool SendMessageAsynch(someipMessage &msg ) = 0;

		/* FUNCTION TO READ A SOMEIP MESSAGE ASYNCH */
		virtual someipMessage ReceiveMessageAsynch() = 0;	


		/* MESSAGE TYPES */

		virtual someipMessage SendRequest(someipMessage &msg) = 0;

		virtual bool SendResponse(someipMessage &msg) = 0;

		/* REQUEST NO RESPONSE */
		virtual bool SendFireAndForget(someipMessage &msg) = 0;

		virtual bool SendNotification(someipMessage &msg) = 0;
};


} // End of Namespace someip

