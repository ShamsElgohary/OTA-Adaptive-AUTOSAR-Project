#pragma once

#include "someip.hpp"


namespace someip {


	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP UDP  ////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

class someipUDP  : public someipConnection  {
	
	protected:

	boost::asio::ip::udp::endpoint udpEndPoint;
	boost::asio::ip::udp::socket udpSocket;
	boost::asio::io_context & udp_io_service;  
	//boost::asio::deadline_timer timer;
	uint16_t port;

	public:

	/* CONSTRUCTOR */
	someipUDP(boost::asio::io_context& io_service, uint16_t port, std::string IPv4 = LOOPBACK_IP);	

	/* CONSTRUCTOR (+ SOMEIP ENDUSER CONSTRUCTOR)*/
	someipUDP(
		boost::asio::io_context& io_service, 
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


} // namespace someip