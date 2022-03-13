#include "SOMEIP_TCP.hpp"


using namespace boost::asio::ip;
using boost::asio::ip::udp;
using boost::asio::ip::address;

using namespace std;

namespace someip
{

		
	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP TCP     /////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

		/* CONSTRUCTOR */
	someipTCP::someipTCP(boost::asio::io_service& io_service, uint16_t port, std::string IPv4)			
		: tcpSocket(io_service)
	{
		auto ip = address::from_string(IPv4);
		this->port = port;
		this->tcpEndPoint = boost::asio::ip::tcp::endpoint(ip, port);
	}


	/* USED TO CONSTRUCT AN INSTANCE IN THE SOMEIP ENDUSER CLASS */
	someipTCP::someipTCP(
		boost::asio::io_service& io_service, 
		uint16_t port, 
		SomeIpConfiguration someipConfig,
		std::string IPv4)		
		: tcpSocket(io_service), someipEndUser(someipConfig)
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

	/* FUNCTION TO SEND A SOME/IP MESSAGE USING TCP SYNCHRONOUS */
	bool someipTCP::SendMessage(someip_Message &msg)
	{
		try {
			
			std::stringstream ss; 
			msg.Serialize(ss);
			std::string mssgBuf = ss.str();
			tcpSocket.write_some( boost::asio::buffer(mssgBuf) );

		}catch ( boost::system::system_error e) {
			std::cout << e.what() << "\n";
			return false; // MESSAGE NOT SENT
		}

		return true;

	}


	/* FUNCTION TO RECEIVE A SOME/IP MESSAGE USING TCP */
	someip_Message someipTCP::ReadMessage()
	{
		someip_Message someipMsg;

		try {
			char buff[512];
			size_t read = tcpSocket.read_some(boost::asio::buffer(buff));
			std::string mssgBuf = buff;
			std::stringstream ss;
			ss<< mssgBuf;
			someipMsg.Deserialize(ss);

		}catch ( boost::system::system_error e) {
			std::cout << e.what() << "\n";
			someipMsg.header;
			// MESSAGE NOT READ
		}

	   return someipMsg;
	}


	bool someipTCP::OpenConnection()
	{
		return true;
	}

	bool someipTCP::CloseConnection()
	{
		boost::system::error_code error;
		tcpSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
		return true;
	}

	


	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  TCP CLIENT   ///////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

	ClientTCP::ClientTCP(boost::asio::io_service& io_service, uint16_t port, std::string IPv4)			
		: someipTCP(io_service, port, IPv4)
	{
		try{
			//CLIENT/PROXY CONNECTION
			tcpSocket.connect(this->tcpEndPoint);
		}
		catch (boost::system::system_error e)
		{
			std::cout << e.what() << "\n";
			std::cout << "CLIENT DIDN'T CONNECT TO SERVER \n";
		}
	}


	/* USED TO CONSTRUCT AN INSTANCE IN THE SOMEIP ENDUSER CLASS */
	ClientTCP::ClientTCP(
		boost::asio::io_service& io_service, 
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
			std::cout << e.what() << "\n";
			std::cout << "CLIENT DIDN'T CONNECT TO SERVER \n";
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
			std::cout << e.what() << "\n";
			std::cout << "CLIENT DIDN'T CONNECT TO SERVER (PROXY CONNECT METHOD) \n";
			opResult = false;

		}

		return opResult;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP TCP SERVER   ////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////



	ServerTCP::ServerTCP(boost::asio::io_service& io_service, uint16_t port, std::string IPv4)			
		: 
		server_io_service(io_service), 
		acceptor(io_service, boost::asio::ip::tcp::endpoint(address::from_string(IPv4), port)), 			
		someipTCP(io_service, port, IPv4)
	{
		// CONSTRUCTOR
	}


	/* USED TO CONSTRUCT AN INSTANCE IN THE SOMEIP ENDUSER CLASS */
	ServerTCP::ServerTCP(
		boost::asio::io_service& io_service, 
		uint16_t port, 
		SomeIpConfiguration someipConfig,
		std::string IPv4)		
		: 	
		server_io_service(io_service), 
		acceptor(io_service, boost::asio::ip::tcp::endpoint(address::from_string(IPv4), port)), 			
		someipTCP(io_service, port, someipConfig, IPv4)
	{	
		// CONSTRUCTOR
	}


	void ServerTCP::ServerListen() 
	{
		try{
			//waiting for connection
      		acceptor.accept(this->tcpSocket);
		}
		catch(...)
		{
			std::cout << " ERROR ACCEPTING CONNECTION \n";
		}
	
	}


} // Namespace someip