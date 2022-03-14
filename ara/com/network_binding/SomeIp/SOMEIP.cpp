#include "SOMEIP.hpp"
#include "SOMEIP_TCP.hpp"



using namespace boost::asio::ip;
using boost::asio::ip::udp;
using boost::asio::ip::address;

using namespace std;

namespace someip
{

	/* USED AS A HANDLE IN THE SOCKET FOR ASYNCHRONOUS MESSAGES */
	void on_send_completed(boost::system::error_code ec, size_t bytes_transferred) {
    if (ec)
        std::cout << "Send failed: " << ec.message() << "\n";
    else
        std::cout << "Send succesful (" << bytes_transferred << " bytes)\n";
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
		shared_ptr<someipEndUser > endUserInstance;

		if (someipConfig.tpType == TransportProtocol::TCP)
		{
			if(someipConfig.endUserType == EndUserType::CLIENT)
			{
				endUserInstance = make_shared<ClientTCP> (io_service, port, someipConfig, IPv4);
			}

			else if(someipConfig.endUserType == EndUserType::SERVER)
			{
				endUserInstance = make_shared<ServerTCP> (io_service, port, someipConfig, IPv4);
			}

			else
			{
				cout<<" UNDEFINED TYPE OF END USER \n";
			}

		}

		else if(someipConfig.tpType == TransportProtocol::UDP)
		{
			if(someipConfig.endUserType == EndUserType::CLIENT)
			{

			}

			else if(someipConfig.endUserType == EndUserType::SERVER)
			{

			}

			else
			{
				cout<<" UNDEFINED TYPE OF END USER \n";
			}
		}

		else
		{
			cout<<" UNDEFINED TYPE OF TRANSPORT PROTOCOL \n";	
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



		
	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP UDP  ////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


	someipUDP::someipUDP(boost::asio::io_service& io_service, uint16_t port)			
		:endpoint(localIp, port ), socket_(io_service, endpoint.protocol() ), timer(io_service)
	{
		/* CONSTRUCTOR */
	}


	/* FUNCTION TO SEND A SOME/IP MESSAGE USING UDP */
	void someipUDP::SendMessage(someip_Message &msg, uint16_t port)
	{
		// /* SERIALIZE MESSAGE */
		// std::stringstream ss;
		// msg.Serialize(ss);
		// /* SENDABLE DATA */
		// std::string data = ss.str();
		// /* SEND DATA */
    	// if( send(sockfd , data.c_str() , strlen(data.c_str() ) , 0) < 0)
    	// {
        // 	perror("Send failed ");
        // 	return;
   	 	// }
	}


	/* FUNCTION TO READ A SOME/IP MESSAGE USING UDP */
	someip_Message someipUDP::ReadMessage()
	{
		// char buffer[size];

		// //Receive a reply from the server
		// if( recv(sockfd , buffer , sizeof(buffer) , 0) < 0)
		// {
		// 	puts("recv failed");
		// }

		// /* ALL RECEIVED DATA IN MESSAGE STREAM */
		// std::string messageReceivedStr = buffer;
		
        // std::stringstream ss;
        // ss<<messageReceivedStr;
	    someip::someip_Message someipMsg;
	    //someipMsg.Deserialize(ss);
	    return someipMsg;
	}

} // Namespace someip