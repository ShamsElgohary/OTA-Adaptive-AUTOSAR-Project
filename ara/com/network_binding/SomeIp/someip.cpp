#include "someip.hpp"


using namespace boost::asio::ip;
using boost::asio::ip::udp;
using boost::asio::ip::address;

using namespace std;

namespace someip
{


	////////////////////////////// HANDLES FOR ASYNCH OPERATIONS ///////////////////////////////////////


		/* USED AS A HANDLE IN THE SOCKET FOR ASYNCHRONOUS MESSAGES */
	void OnSendCompleted(boost::system::error_code ec, size_t bytes_transferred) {
    if (ec)
        std::cout << "Send failed: " << ec.message() << "\n";
    else
        std::cout << "Send succesful (" << bytes_transferred << " bytes)\n";
	}

			/* USED AS A HANDLE IN THE SOCKET FOR ASYNCHRONOUS MESSAGES */
	void OnReceiveCompleted(boost::system::error_code ec, size_t bytes_transferred) {
    if (ec)
        std::cout << "Receive Failed: " << ec.message() << "\n";
    else
        std::cout << "Receive Successful (" << bytes_transferred << " bytes)\n";
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP CLIENT  	/////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


	someipEndUser::someipEndUser(SomeIpConfiguration someipConfig)
	{
		this->tpType = someipConfig.tpType;
		this->endUserType = someipConfig.endUserType;
	}

	someipEndUser::someipEndUser()
	{
		/* DEFAULT CONSTRUCTOR */
	}

	someipEndUser::~someipEndUser()
	{
		/* DEFAULT DESTRUCTOR */
	}

	std::shared_ptr<someipEndUser> someipEndUser::SetSomeIpConfiguration(
		boost::asio::io_service& io_service, 
		uint16_t port, 	 
		SomeIpConfiguration someipConfig, 
		std::string IPv4 )
		
	{
		shared_ptr<someipEndUser> endUserInstance;

		if (someipConfig.tpType == TransportProtocol::TCP)
		{
			if (someipConfig.endUserType == EndUserType::CLIENT)
			{
				endUserInstance = make_shared<ClientTCP>(io_service, port, someipConfig, IPv4);
			}

			else if (someipConfig.endUserType == EndUserType::SERVER)
			{
				endUserInstance = make_shared<ServerTCP>(io_service, port, someipConfig, IPv4);
			}

			else
			{
				cout << " UNDEFINED TYPE OF END USER \n";
			}
		}

		else if (someipConfig.tpType == TransportProtocol::UDP)
		{
			if (someipConfig.endUserType == EndUserType::CLIENT)
			{
			}

			else if (someipConfig.endUserType == EndUserType::SERVER)
			{
			}

			else
			{
				cout << " UNDEFINED TYPE OF END USER \n";
			}
		}

		else
		{
			cout << " UNDEFINED TYPE OF TRANSPORT PROTOCOL \n";
		}

		return endUserInstance;
	}

	/* TCP OR UDP MESSAGES? */
	TransportProtocol someipEndUser::GetTransportProtocolType()
	{
		return this->tpType;
	}

	/* END USER TYPE? */
	EndUserType someipEndUser::GetEndUserType()
	{
		return this->endUserType;
	}

	/* CONNECT PROXY TO SERVER */
	bool someipEndUser::ProxyConnect()
	{
		// METHOD RELATED TO CLIENTS ONLY 
		std::cout<< " WRONG INSTANCE CALLED... \n";
		return false;
	}	

	/* SERVER LISTENING */
	void someipEndUser::ServerListen()
	{
		std::cout<< " WRONG INSTANCE CALLED... \n";
		// METHOD RELATED TO SERVER ONLY
	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// 		TCP 	/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




	/////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////  someipTCP CLASS    ///////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

		/* CONSTRUCTOR */
	someipTCP::someipTCP(boost::asio::io_service& io_service, uint16_t port, std::string IPv4)			
		: tcpSocket(io_service), tcp_io_service(io_service)
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
		: tcpSocket(io_service), tcp_io_service(io_service), someipEndUser(someipConfig) 
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
	someip_Message someipTCP::ReceiveMessage()
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



	/* FUNCTION TO SEND A SOME/IP MESSAGE USING TCP SYNCHRONOUS */
	bool someipTCP::SendMessageAsynch(someip_Message &msg)
	{
		try {
			
			std::stringstream ss; 
			msg.Serialize(ss);
			std::string mssgBuf = ss.str();
			tcpSocket.async_write_some( boost::asio::buffer(mssgBuf), &OnSendCompleted );
			// IMPORTANT FOR ASYNCHRONOUS OPERATIONS 
			tcp_io_service.run();

		}catch ( boost::system::system_error e) {
			std::cout << e.what() << "\n";
			return false; // MESSAGE NOT SENT
		}

		return true;

	}


	/* FUNCTION TO RECEIVE A SOME/IP MESSAGE USING TCP */
	someip_Message someipTCP::ReceiveMessageAsynch()
	{
		someip_Message someipMsg;

		try {
			char buff[512];
			tcpSocket.async_read_some(boost::asio::buffer(buff), &OnReceiveCompleted);
			// IMPORTANT FOR ASYNCHRONOUS OPERATIONS 
			tcp_io_service.run();
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
		tcpSocket.close();
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
		catch( boost::system::system_error e) {
			std::cout << e.what() << "\n";
			std::cout << " ERROR ACCEPTING CONNECTION \n";
		}
	
	}

	


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// 		UDP   	 /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		
	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP UDP  ////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////




	/* CONSTRUCTOR */
	someipUDP::someipUDP(boost::asio::io_service& io_service, uint16_t port, std::string IPv4)
		: udpSocket(io_service)
	{
		auto ip = address::from_string(IPv4);
		this->port = port;
		this->udpEndPoint = boost::asio::ip::udp::endpoint(ip, port);
	}


	/* CONSTRUCTOR (+ SOMEIP ENDUSER CONSTRUCTOR)*/
	someipUDP::someipUDP(
		boost::asio::io_service& io_service, 
		uint16_t port, 
		SomeIpConfiguration someipConfig,
		std::string IPv4)
		: udpSocket(io_service), someipEndUser(someipConfig)
	{
		auto ip = address::from_string(IPv4);
		this->port = port;
		this->udpEndPoint = boost::asio::ip::udp::endpoint(ip, port);
	}
	
	someipUDP::~someipUDP()
	{
		/* DEFAULT DESTRUCTOR */
	}


	/* FUNCTION TO SEND A SOMEIP MESSAGE USING UDP */
	bool someipUDP::SendMessage(someip_Message &msg)
	{
		try {
			// RECEIVER ENDPOINT
			boost::asio::ip::udp::endpoint receiver_ep(this->udpEndPoint);
			std::stringstream ss; 
			msg.Serialize(ss);
			std::string mssgBuf = ss.str();
			udpSocket.send_to(boost::asio::buffer(mssgBuf), receiver_ep);

		}catch ( boost::system::system_error e) {
			std::cout << e.what() << "\n";
			return false; // MESSAGE NOT SENT
		}

		return true;

	}


	/* FUNCTION TO READ A SOMEIP MESSAGE USING UDP */
	someip_Message someipUDP::ReceiveMessage()
	{

		someip_Message someipMsg;

		try {
			boost::asio::ip::udp::endpoint sender_ep;
			char buff[512];
			size_t read = udpSocket.receive_from(boost::asio::buffer(buff), sender_ep);
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


		/* FUNCTION TO SEND A SOMEIP MESSAGE USING TCP ASYNCH */
	bool someipUDP::SendMessageAsynch(someip_Message &msg)
	{

		return true;
	}

	/* FUNCTION TO READ A SOMEIP MESSAGE USING TCP ASYNCH*/
	someip_Message someipUDP::ReceiveMessageAsynch()
	{
		someip_Message someipMsg;

		try {
			boost::asio::ip::udp::endpoint sender_ep;
			char buff[512];
			size_t read = udpSocket.receive_from(boost::asio::buffer(buff), sender_ep);
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

	bool someipUDP::OpenConnection()
	{
		return true;
	}

	bool someipUDP::CloseConnection()
	{
		boost::system::error_code error;
		udpSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
		udpSocket.close();
		return true;
	}

	/* SERVER LISTENING */
	void someipUDP::ServerListen()
	{

	}

	/* CONNECT PROXY TO SERVER */
	bool someipUDP::ProxyConnect()
	{
		return true;
	}

} // Namespace someip