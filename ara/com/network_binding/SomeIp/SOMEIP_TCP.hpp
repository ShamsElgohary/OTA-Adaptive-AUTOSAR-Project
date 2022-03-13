#include "SOMEIP.hpp"

using namespace someip;

namespace someip
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP TCP     /////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

class someipTCP : public someipEndUser {
	
	protected:
	
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
	bool SendMessage(someip_Message &msg);

	/* FUNCTION TO READ A SOMEIP MESSAGE USING TCP */
	someip_Message ReadMessage();

	/* OPEN SOCKET CONNECTION*/
	bool OpenConnection();

	/* CLOSE SOCKET CONNECTION */
	bool CloseConnection();


};


	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP TCP SERVER   ////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


class ServerTCP : public someipTCP {
	
	private:
	boost::asio::io_service & server_io_service;  
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


	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP TCP CLIENT   ////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


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



} // NAMESPACE SOMEIP