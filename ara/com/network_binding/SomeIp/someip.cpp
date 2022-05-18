#include "someipUDP.hpp"
#include "someipTCP.hpp"
#include "TLS/someipSecurity.hpp"
#include "someip.hpp"
#include "../../../../utility/general.hpp"

using boost::asio::ip::udp;
using boost::asio::ip::address;
using namespace std;
using namespace someip::security;
using namespace boost::asio::ip;


namespace someip 
{

	////////////////////////////// HANDLES FOR ASYNCH OPERATIONS ///////////////////////////////////////


		/* USED AS A HANDLE IN THE SOCKET FOR ASYNCHRONOUS MESSAGES */
	void OnSendCompleted(boost::system::error_code ec, size_t bytes_transferred) {
    if (ec)
        std::cout << " [someip]Send failed: " << ec.message() << "\n";
    else
        std::cout << " [someip]Send succesful (" << bytes_transferred << " bytes)\n";
	}

			/* USED AS A HANDLE IN THE SOCKET FOR ASYNCHRONOUS MESSAGES */
	void OnReceiveCompleted(boost::system::error_code ec, size_t bytes_transferred) {
    if (ec)
        std::cout << " [someip]Receive Failed: " << ec.message() << "\n";
    else
        std::cout << " [someip]Receive Successful (" << bytes_transferred << " bytes)\n";
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP CLIENT  	/////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


	someipConnection::someipConnection(SomeIpConfiguration someipConfig)
	{
		this->tpType = someipConfig.tpType;
		this->endUserType = someipConfig.endUserType;
	}

	someipConnection::someipConnection()
	{
		/* DEFAULT CONSTRUCTOR */
	}

	someipConnection::~someipConnection()
	{
		/* DEFAULT DESTRUCTOR */
	}

	std::shared_ptr<someipConnection> someipConnection::SetSomeIpConfiguration(
		boost::asio::io_context & io_service, 
		uint16_t port, 	 
		SomeIpConfiguration someipConfig, 
		std::string IPv4 )
		
	{
		shared_ptr<someipConnection> endUserInstance;


	///////////////////// RAW ////////////////////////
	if(someipConfig.securityType == SecurityType::RAW)
	{
		if (someipConfig.tpType == TransportProtocol::TCP)
		{
			if (someipConfig.endUserType == EndUserType::CLIENT)
				endUserInstance = make_shared<ClientTCP>(io_service, port, someipConfig, IPv4);
			
			else if (someipConfig.endUserType == EndUserType::SERVER)
				endUserInstance = make_shared<ServerTCP>(io_service, port, someipConfig, IPv4);
			
		}

		else if (someipConfig.tpType == TransportProtocol::UDP)
		{
			if (someipConfig.endUserType == EndUserType::CLIENT)
				endUserInstance = make_shared<someipUDP>(io_service, port, someipConfig, IPv4);
		

			else if (someipConfig.endUserType == EndUserType::SERVER)
				endUserInstance = make_shared<someipUDP>(io_service, port, someipConfig, IPv4);
		}
	}

	///////////////////// TLS ////////////////////////
	else if (someipConfig.securityType == SecurityType::TLS)
	{
		if (someipConfig.endUserType == EndUserType::CLIENT)
			{	
				std::string CertificatePath = CUSTOMIZED_PROJECT_PATH + "/ara/com/network_binding/SomeIp/TLS/Certificates/server.crt"; 

				boost::asio::ssl::context ctx(boost::asio::ssl::context::tls);
				endUserInstance = make_shared<ClientTLS>(io_service, ctx , port, CertificatePath, IPv4);
			}

		else if (someipConfig.endUserType == EndUserType::SERVER)
			{
				std::string CertificateDir = CUSTOMIZED_PROJECT_PATH + "/ara/com/network_binding/SomeIp/TLS/Certificates"; 

				endUserInstance = make_shared<ServerTLS>(io_service, port, CertificateDir, IPv4);
			}
		
	}
		else
		{
			cout << "[someip] UNDEFINED TYPE OF TRANSPORT PROTOCOL \n";
		}

		return endUserInstance;
	}

	/* TCP OR UDP MESSAGES? */
	TransportProtocol someipConnection::GetTransportProtocolType()
	{
		return this->tpType;
	}

	/* END USER TYPE? */
	EndUserType someipConnection::GetEndUserType()
	{
		return this->endUserType;
	}

	/* CONNECT PROXY TO SERVER */
	bool someipConnection::ProxyConnect()
	{
		// METHOD RELATED TO CLIENTS ONLY 
		std::cout<< " [someip] WRONG INSTANCE CALLED... \n";
		return false;
	}	

	/* SERVER LISTENING */
	void someipConnection::ServerListen()
	{
		std::cout<< " [someip] WRONG INSTANCE CALLED... \n";
		// METHOD RELATED TO SERVER ONLY
	}

} // Namespace someip