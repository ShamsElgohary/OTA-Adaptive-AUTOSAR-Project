#include "someipTCP.hpp"

using namespace boost::asio::ip;
using boost::asio::ip::address;
using namespace std;


namespace someip {


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// 		TCP 	/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////  someipTCP CLASS    ///////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

	/* CONSTRUCTOR */
	someipTCP::someipTCP(boost::asio::io_context & io_service, uint16_t port, std::string IPv4)			
		: tcpSocket(io_service), tcp_io_service(io_service)
	{
		auto ip = address::from_string(IPv4);
		this->port = port;
		this->tcpEndPoint = boost::asio::ip::tcp::endpoint(ip, port);
	}

	/* USED TO CONSTRUCT AN INSTANCE IN THE SOMEIP ENDUSER CLASS */
	someipTCP::someipTCP(
		boost::asio::io_context & io_service, 
		uint16_t port, 
		SomeIpConfiguration someipConfig,
		std::string IPv4)		
		: tcpSocket(io_service), tcp_io_service(io_service), someipConnection(someipConfig) 
	{
		auto ip = address::from_string(IPv4);
		this->port = port;
		this->tcpEndPoint = boost::asio::ip::tcp::endpoint(ip, port);
	}

	/* DESTRUCTOR */
	someipTCP::~someipTCP()
	{
		this->CloseConnection();
		tcpSocket.close();
	}


	someipMessage someipTCP::SendRequest(someipMessage &req)
	{
		req.header.setMessageType( MessageType::REQUEST );
		/* Report Message Info */
		//ReportMessageInfo(req);
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


    bool someipTCP::SendResponse(someipMessage &responseMsg)
	{
		responseMsg.header.setMessageType( MessageType::RESPONSE );
		/* Report Message Info */
		//ReportMessageInfo(responseMsg);
		bool operationStatus = this->SendMessage(responseMsg);
		return operationStatus;
	}


	/* REQUEST NO RESPONSE */
	bool someipTCP::SendFireAndForget(someipMessage &msg)
	{
		msg.header.setMessageType( MessageType::REQUEST_NO_RETURN );
		/* Report Message Info */
		//ReportMessageInfo(msg);
		this->SendMessage(msg);
		return true;
	}

	bool someipTCP::SendNotification(someipMessage &msg)
	{
		msg.header.setMessageType( MessageType::NOTIFICATION );
		/* Report Message Info */
		//ReportMessageInfo(msg);
		this->SendMessage(msg);
		return true;
	}

	/* FUNCTION TO SEND A SOME/IP MESSAGE USING TCP SYNCHRONOUS */
	bool someipTCP::SendMessage(someipMessage &msg)
	{
		try {
			Serializer serializer;
			std::stringstream ss;
			// SERIALIZE SOMEIP MESSAGE STRUCT  
			serializer.Serialize(ss, &msg);
			std::string mssgBuf = ss.str();
			tcpSocket.write_some( boost::asio::buffer(mssgBuf) );

		}catch ( boost::system::system_error e) {
			std::cout << "[someip] " << e.what() << "\n";
			return false; // MESSAGE NOT SENT
		}

		return true;
	}


	/* FUNCTION TO RECEIVE A SOME/IP MESSAGE USING TCP */
	someipMessage someipTCP::ReceiveMessage()
	{
		someipMessage someipMsg;

		try {

			char buff[SOMEIP_BUFFER_SIZE];
			size_t read = tcpSocket.read_some(boost::asio::buffer(buff));
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



	/* FUNCTION TO SEND A SOME/IP MESSAGE USING TCP SYNCHRONOUS */
	bool someipTCP::SendMessageAsynch(someipMessage &msg)
	{
		try {
			
			Serializer serializer;
			std::stringstream ss;
			// SERIALIZE SOMEIP MESSAGE STRUCT  
			serializer.Serialize(ss, &msg);
			std::string mssgBuf = ss.str();
			tcpSocket.async_write_some( boost::asio::buffer(mssgBuf), &OnSendCompleted );
			// IMPORTANT FOR ASYNCHRONOUS OPERATIONS 
			tcp_io_service.run();

		}catch ( boost::system::system_error e) {
			std::cout << "[someip] " << e.what() << "\n";
			return false; // MESSAGE NOT SENT
		}

		return true;
	}


	/* FUNCTION TO RECEIVE A SOME/IP MESSAGE USING TCP */
	someipMessage someipTCP::ReceiveMessageAsynch()
	{
		someipMessage someipMsg;

		try {

			char buff[SOMEIP_BUFFER_SIZE];
			tcpSocket.async_read_some(boost::asio::buffer(buff), &OnReceiveCompleted);
			// IMPORTANT FOR ASYNCHRONOUS OPERATIONS 
			tcp_io_service.run();
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

	bool someipTCP::CloseConnection()
	{
		boost::system::error_code error;
		tcpSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
		tcpSocket.close();
		return true;
	}

	
	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP TCP SERVER   ////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


	ServerTCP::ServerTCP(boost::asio::io_context & io_service, uint16_t port, std::string IPv4)			
		: 
		acceptor(io_service, boost::asio::ip::tcp::endpoint(address::from_string(IPv4), port)), 			
		someipTCP(io_service, port, IPv4)
	{
		// CONSTRUCTOR
	}


	/* USED TO CONSTRUCT AN INSTANCE IN THE SOMEIP ENDUSER CLASS */
	ServerTCP::ServerTCP(
		boost::asio::io_context & io_service, 
		uint16_t port, 
		SomeIpConfiguration someipConfig,
		std::string IPv4)		
		: 	 
		acceptor(io_service, boost::asio::ip::tcp::endpoint(address::from_string(IPv4), port)), 			
		someipTCP(io_service, port, someipConfig, IPv4)
	{	
		// CONSTRUCTOR
	}


	void ServerTCP::ServerListen() 
	{
		try
		{
			//waiting for connection
      		acceptor.accept(this->tcpSocket);
		}
		catch( boost::system::system_error e) {
			std::cout << "[someip] " << e.what() << "\n";
			std::cout << "[someip] ERROR ACCEPTING CONNECTION \n";
		}
	
	}



	/////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////// SOMEIP TCP CLIENT   /////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

	ClientTCP::ClientTCP(boost::asio::io_context & io_service, uint16_t port, std::string IPv4)			
		: someipTCP(io_service, port, IPv4)
	{
		try{
			//CLIENT/PROXY CONNECTION
			tcpSocket.connect(this->tcpEndPoint);
		}
		catch (boost::system::system_error e)
		{
			std::cout << "[someip] " << e.what() << "\n";
			std::cout << "[someip] CLIENT DIDN'T CONNECT TO SERVER \n";
		}
	}


	/* USED TO CONSTRUCT AN INSTANCE IN THE SOMEIP ENDUSER CLASS */
	ClientTCP::ClientTCP(
		boost::asio::io_context & io_service, 
		uint16_t port, 
		SomeIpConfiguration someipConfig,
		std::string IPv4)		
		: someipTCP(io_service, port, someipConfig, IPv4)
	{
		try{
			//CLIENT/PROXY CONNECTION
			tcpSocket.connect(this->tcpEndPoint);
		}
		catch (boost::system::system_error e)
		{
			std::cout << "[someip] " << e.what() << "\n";
			std::cout << "[someip] CLIENT DIDN'T CONNECT TO SERVER \n";
		}
	}


	bool ClientTCP::ProxyConnect()
	{
		bool opResult = true;
		try{

			//CLIENT/PROXY CONNECTION
			tcpSocket.connect(this->tcpEndPoint);
		}
		catch (boost::system::system_error e)
		{
			std::cout << "[someip] " << e.what() << "\n";
			std::cout << "[someip] CLIENT DIDN'T CONNECT TO SERVER (PROXY CONNECT METHOD) \n";
			opResult = false;

		}

		return opResult;
	}










}