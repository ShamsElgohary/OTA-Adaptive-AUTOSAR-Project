#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <functional>
#include "SOMEIP_Message.hpp"


namespace someip {
					
static const std::string LOCAL_IP = "127.0.0.1";
static const std::string MULTICAST_IP = "239.255.0.1";	// mulicast address to use. see: http://en.wikipedia.org/wiki/Multicast_address
static const int MULTICAST_PORT = 5000;

static const boost::asio::ip::address& localIp = boost::asio::ip::address::from_string(LOCAL_IP);

class someipEndUser{

	private:
		boost::asio::ip::udp::endpoint endpoint;
		boost::asio::ip::udp::socket socket;
		boost::asio::deadline_timer timer;
		std::string message;
		uint16_t port;

		/* ERROR HANDLING METHODS */
		void handle_send_to(const boost::system::error_code& error);
		void handle_timeout(const boost::system::error_code& error);
		//void on_send_completed(boost::system::error_code ec, size_t bytes_transferred);

	public:

		someipEndUser(
		boost::asio::io_service& io_service,	// io_service to use
		uint16_t port); 						// the port where the service listens on


		/* FUNCTION TO SEND A SOMEIP MESSAGE USING TCP */
		void SendMessageTCP(someip_Message &msg, uint16_t port);

		/* FUNCTION TO READ A SOMEIP MESSAGE USING TCP */
		someip_Message ReadMessageTCP(boost::asio::ip::tcp::socket & socket);

		/* FUNCTION TO SEND A SOMEIP MESSAGE USING UDP */
		void SendMessageUDP(someip_Message &msg, uint16_t port);

		/* FUNCTION TO READ A SOMEIP MESSAGE USING UDP */
		someip_Message ReadMessageUDP();

};



} // End of Namespace someip


