#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <functional>

#include "someipMessage.hpp"


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


typedef struct
{
	TransportProtocol tpType; 
	EndUserType endUserType;
}SomeIpConfiguration;



	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP CONNECTION    ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


class someipConnection{
	
	protected: 

		InstanceID instance;
		TransportProtocol tpType;
		EndUserType endUserType;
		static RequestID RequestId;
	
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





	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP TCP     /////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

class someipTCP : public someipConnection {
	
	protected:
	
	boost::asio::io_service & tcp_io_service;  
	boost::asio::ip::tcp::endpoint tcpEndPoint;
	boost::asio::ip::tcp::socket tcpSocket;
	uint16_t port;

	public:

	/* CONSTRUCTOR */
	someipTCP(boost::asio::io_service& io_service, uint16_t port, std::string IPv4 = LOOPBACK_IP);	

	/* CONSTRUCTOR (+ SOMEIP ENDUSER CONSTRUCTOR)*/
	someipTCP(
		boost::asio::io_service& io_service, 
		uint16_t port, 
		SomeIpConfiguration someipConfig,
		std::string IPv4 = LOOPBACK_IP);	


	/* DESTRUCTOR */
	~someipTCP();

	/* FUNCTION TO SEND A SOMEIP MESSAGE USING TCP */
	bool SendMessage(someipMessage &msg) override;

	/* FUNCTION TO READ A SOMEIP MESSAGE USING TCP */
	someipMessage ReceiveMessage();

	/* FUNCTION TO SEND A SOMEIP MESSAGE USING TCP ASYNCH */
	bool SendMessageAsynch(someipMessage &msg);

	/* FUNCTION TO READ A SOMEIP MESSAGE USING TCP ASYNCH*/
	someipMessage ReceiveMessageAsynch();

	/* OPEN SOCKET CONNECTION*/
	bool OpenConnection();

	/* CLOSE SOCKET CONNECTION */
	bool CloseConnection();

	/* MESSAGE TYPES */

	someipMessage SendRequest(someipMessage &msg);

    bool SendResponse(someipMessage &msg);

	/* REQUEST NO RESPONSE */
	bool SendFireAndForget(someipMessage &msg);

	bool SendNotification(someipMessage &msg);

};



    ///////////////////////////////////  SOMEIP TCP SERVER   ////////////////////////////////////////
 
class ServerTCP : public someipTCP {
	
	private:
	boost::asio::ip::tcp::acceptor acceptor;  

	public:

	/* CONSTRUCTOR PERFORMS SPECIFIC OPERATIONS (SERVER )*/
	ServerTCP(boost::asio::io_service& io_service, uint16_t port, std::string IPv4 = LOOPBACK_IP);	

	/* CONSTRUCTOR (+ SOMEIP ENDUSER )*/
	ServerTCP(
		boost::asio::io_service& io_service, 
		uint16_t port, 
		SomeIpConfiguration someipConfig,
		std::string IPv4 = LOOPBACK_IP);	

	/* SERVER LISTENING */
	void ServerListen();	

};


    ///////////////////////////////////  SOMEIP TCP CLIENT   ////////////////////////////////////////

class ClientTCP : public someipTCP {
	
	private:


	public:

	/* CONSTRUCTOR PERFORMS SPECIFIC OPERATIONS (CLIENT )*/
	ClientTCP(boost::asio::io_service& io_service, uint16_t port, std::string IPv4 = LOOPBACK_IP);	

	/* CONSTRUCTOR (+ SOMEIP ENDUSER )*/
	ClientTCP(
		boost::asio::io_service& io_service, 
		uint16_t port, 
		SomeIpConfiguration someipConfig,
		std::string IPv4 = LOOPBACK_IP);	
	

	/* CONNECT PROXY */
	bool ProxyConnect();	

};




	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP UDP  ////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

class someipUDP  : public someipConnection  {
	
	protected:

	boost::asio::ip::udp::endpoint udpEndPoint;
	boost::asio::ip::udp::socket udpSocket;
	boost::asio::io_service & udp_io_service;  
	//boost::asio::deadline_timer timer;
	uint16_t port;

	public:

	/* CONSTRUCTOR */
	someipUDP(boost::asio::io_service& io_service, uint16_t port, std::string IPv4 = LOOPBACK_IP);	

	/* CONSTRUCTOR (+ SOMEIP ENDUSER CONSTRUCTOR)*/
	someipUDP(
		boost::asio::io_service& io_service, 
		uint16_t port, 
		SomeIpConfiguration someipConfig,
		std::string IPv4 = LOOPBACK_IP);	

	/* DESTRUCTOR */
	~someipUDP();

	/* FUNCTION TO SEND A SOMEIP MESSAGE USING UDP */
	bool SendMessage(someipMessage &msg);

	/* FUNCTION TO READ A SOMEIP MESSAGE USING UDP */
	someipMessage ReceiveMessage();

	/* FUNCTION TO SEND A SOMEIP MESSAGE USING TCP ASYNCH */
	bool SendMessageAsynch(someipMessage &msg);

	/* FUNCTION TO READ A SOMEIP MESSAGE USING TCP ASYNCH*/
	someipMessage ReceiveMessageAsynch();

	/* OPEN SOCKET CONNECTION*/
	bool OpenConnection();

	/* CLOSE SOCKET CONNECTION */
	bool CloseConnection();

	/* SERVER LISTENING */
	void ServerListen();

	/* CONNECT PROXY TO SERVER */
	bool ProxyConnect();


	someipMessage SendRequest(someipMessage &msg);

    bool SendResponse(someipMessage &msg);

	/* REQUEST NO RESPONSE */
	bool SendFireAndForget(someipMessage &msg);

	bool SendNotification(someipMessage &msg);

};




} // End of Namespace someip

