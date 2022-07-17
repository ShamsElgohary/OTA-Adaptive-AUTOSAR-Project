#include "someipUDP.hpp"

using namespace boost::asio::ip;
using boost::asio::ip::udp;
using boost::asio::ip::address;
using namespace std;


namespace someip{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// 		UDP   	 /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



		
	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP UDP  ////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////



	/* CONSTRUCTOR (+ SOMEIP ENDUSER CONSTRUCTOR)*/
	someipUDP::someipUDP(
		boost::asio::io_context & io_service, 
		uint16_t port, 
		SomeIpConfiguration someipConfig,
		std::string IPv4)
		: udpSocket(io_service, udpEndPoint.protocol()), udp_io_service(io_service), someipConnection(someipConfig)
	{
		auto ip = address::from_string(IPv4);
		this->port = port;
		this->udpEndPoint = boost::asio::ip::udp::endpoint(ip, port);

		if (someipConfig.endUserType == EndUserType::SERVER)
		{
			// BIND SOCKET TO THE ENDPOINT
			udpSocket.bind(udpEndPoint);
		}
	}
	
	someipUDP::~someipUDP()
	{
		/* DEFAULT DESTRUCTOR */
	}

	someipMessage someipUDP::SendRequest(someipMessage &req)
	{
		req.header.setMessageType( MessageType::REQUEST );

		/* SEND MESSAGE */
		this->SendMessage(req);
		/* RESPONSE */
		someipMessage responseMsg = this->ReceiveMessage();
		if( responseMsg.header.getMessageType() != MessageType::RESPONSE )
		{
			//std::cout<< "[someip] MESSAGE TYPE ISN'T RESPONSE MESSAGE " << std::endl;
			/* CONTAINS HEADER WITH ERROR CODE (DEFAULT CONSTRUCTOR) */
			//someipMessage msg;
			//return msg;
		}

		return responseMsg;
	}


    bool someipUDP::SendResponse(someipMessage &responseMsg)
	{
		responseMsg.header.setMessageType( MessageType::RESPONSE );

		bool operationStatus = this->SendMessage(responseMsg);
		return operationStatus;
	}

	/* REQUEST NO RESPONSE */
	bool someipUDP::SendFireAndForget(someipMessage &msg)
	{
		msg.header.setMessageType( MessageType::REQUEST_NO_RETURN );

		this->SendMessage(msg);
		return true;
	}

	bool someipUDP::SendNotification(someipMessage &msg)
	{
		msg.header.setMessageType( MessageType::NOTIFICATION );

		this->SendMessage(msg);
		return true;
	}
	
	/* FUNCTION TO SEND A SOMEIP MESSAGE USING UDP */
	bool someipUDP::SendMessage(someipMessage &msg)
	{
		/* Report Message Info */
		//ReportMessageInfo(msg);

		try {
			// RECEIVER ENDPOINT
			Serializer serializer;
			std::stringstream ss;
			// SERIALIZE SOMEIP MESSAGE STRUCT  
			serializer.Serialize(ss, &msg);
			std::string mssgBuf = ss.str();
			udpSocket.send_to(boost::asio::buffer(mssgBuf), this->udpEndPoint);

		}catch ( boost::system::system_error e) {
			std::cout << "[someip] " << e.what() << "\n";
			return false; // MESSAGE NOT SENT
		}

		return true;
	}


	/* FUNCTION TO READ A SOMEIP MESSAGE USING UDP */
	someipMessage someipUDP::ReceiveMessage()
	{
		someipMessage someipMsg;

		try {
			char buff[SOMEIP_BUFFER_SIZE];
			size_t read = udpSocket.receive_from(boost::asio::buffer(buff), this->udpEndPoint);
			std::string mssgBuf = buff;
			std::stringstream ss;
			ss<< mssgBuf;
			Deserializer deserializer;
			// DESERIALIZE SOMEIP MESSAGE STRUCT  
			deserializer.Deserialize(ss, &someipMsg);
		}catch ( boost::system::system_error e) {
			std::cout << "[someip] " << e.what() << "\n";
			someipMsg.header;
			// MESSAGE NOT READ
		}

	   return someipMsg;
	}


	/* FUNCTION TO SEND A SOMEIP MESSAGE USING TCP ASYNCH */
	bool someipUDP::SendMessageAsynch(someipMessage &msg)
	{
		this->SendMessage(msg);
		return true;
	}


	
	/* FUNCTION TO READ A SOMEIP MESSAGE USING TCP ASYNCH*/
	someipMessage someipUDP::ReceiveMessageAsynch()
	{
		someipMessage someipMsg;

		try {
			char buff[SOMEIP_BUFFER_SIZE];
			size_t read = udpSocket.receive_from(boost::asio::buffer(buff), this->udpEndPoint);
			std::string mssgBuf = buff;
			std::stringstream ss;
			ss<< mssgBuf;
			Deserializer deserializer;
			// DESERIALIZE SOMEIP MESSAGE STRUCT  
			deserializer.Deserialize(ss, &someipMsg);
		}catch ( boost::system::system_error e) {
			std::cout << "[someip] " << e.what() << "\n";
			someipMsg.header;
			// MESSAGE NOT READ
		}

	   return someipMsg;
	}


	bool someipUDP::CloseConnection()
	{
		boost::system::error_code error;
		udpSocket.shutdown(boost::asio::ip::udp::socket::shutdown_both, error);
		udpSocket.close();
		return true;
	}

	/* SERVER LISTENING */
	void someipUDP::ServerListen()
	{
		// std::cout<< "NO CONNECTION ESTABLISHMENT IN UDP (CONNECTIONLESS) "<<std::endl;
	}

	/* CONNECT PROXY TO SERVER */
	bool someipUDP::ProxyConnect()
	{
		// std::cout<< "NO CONNECTION ESTABLISHMENT IN UDP (CONNECTIONLESS) "<<std::endl;
		return true;
	}


} // namespace someip