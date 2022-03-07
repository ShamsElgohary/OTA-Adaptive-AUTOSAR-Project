#include "SOMEIP.hpp"

using namespace std;

namespace someip
{
    someipHeader::someipHeader() 
    {
    interfaceVersion = 0;
    messageID = requestID = interfaceVersion = 0;
    returnCode = ReturnCode::E_OK;
    messageType = MessageType::ERROR;
    }   

	someipHeader::someipHeader(MessageID messageID, RequestID requestID, InterfaceVersion interfaceVersion, 
                            MessageType messageType, ReturnCode returnCode) 
    {
		interfaceVersion = 0;
		messageID = messageID;
		requestID = requestID;
		interfaceVersion = interfaceVersion;
		messageType = messageType;
		returnCode = returnCode;
	}

    MessageID someipHeader::getMessageID() const {
		return messageID;
	}

    MessageType someipHeader::getMessageType() const {
		return messageType;
	}

	MethodID someipHeader::getMethodID() const {
		return someip::getMethodID( messageID );
	}

    	void someipHeader::setMessageType(MessageType messageType) {
		messageType = messageType;
	}

	bool someipHeader::isErrorType() const {
		return getMessageType() == MessageType::ERROR;
	}

	void someipHeader::setMessageID(MessageID messageID) {
		messageID = messageID;
	}

	void someipHeader::setServiceID(ServiceID serviceID) {
		messageID = someip::getMessageID( serviceID, getMethodID() );
	}

	ServiceID someipHeader::getServiceID() const {
		return someip::getServiceID(messageID);
	}

	void someipHeader::setMethodID(MethodID MethodID) {
		messageID = someip::getMessageID(getServiceID(), MethodID);
	}

	bool someipHeader::isReply() const {
		return ( (messageType == MessageType::RESPONSE) || (messageType == MessageType::ERROR) );
	}

	bool someipHeader::isNotification() const {
		return messageType == MessageType::NOTIFICATION;
	}

	bool someipHeader::isRequestWithReturn() const {
		return messageType == MessageType::REQUEST;
	}

	void someipHeader::setRequestID(RequestID id) {
		requestID = id;
	}

	RequestID someipHeader::getRequestID() const {
		return requestID;
	}

	size_t someipHeader::getLength() {
        //return length;
		return sizeof(someipHeader);
	}

    bool someipHeader::operator==(const someipHeader& right) const {
		if (messageID != right.messageID)
			return false;
		if (requestID != right.requestID)
			return false;
		if (protocolVersion != right.protocolVersion)
			return false;
		if (interfaceVersion != right.interfaceVersion)
			return false;
		if (messageType != right.messageType)
			return false;
		if (returnCode != right.returnCode)
			return false;
		return true;
	}

    someipHeader& someipHeader::operator=(const someipHeader& right) 
    {
		messageID = right.messageID;
		requestID = right.requestID;
		interfaceVersion = right.interfaceVersion;
		messageType = right.messageType;
		returnCode = right.returnCode;
		protocolVersion = right.protocolVersion;
		return *this;
    }

	/* Used as handle in the socket */
	void on_send_completed(boost::system::error_code ec, size_t bytes_transferred) {
    if (ec)
        std::cout << "Send failed: " << ec.message() << "\n";
    else
        std::cout << "Send succesful (" << bytes_transferred << " bytes)\n";
	}

	
	/* FUNCTION TO SEND A someip MESSAGE */
	void SendsomeipMessage(someip_Message &msg, EndPoint rawEP)
	{
		//GetDestination(Header.serviceId,instanceId);
		boost::asio::io_service io_service;
		//socket creation
		boost::asio::ip::tcp::socket socket(io_service);
		auto ip = boost::asio::ip::address::from_string(rawEP.raw_ip_address);
		//connection
		socket.connect( boost::asio::ip::tcp::endpoint( ip, rawEP.port_num ));
		boost::asio::streambuf buf;
		std::ostream oss(&buf);
		// ostream = stringstream 
		std:stringstream ss; 
		msg.Serialize(ss);
		oss << ss.rdbuf();
		// send stream buffer 
     	boost::system::error_code error;
     	boost::asio::async_write( socket, buf, on_send_completed);
	}

	/* FUNCTION TO RECEIVE A someip MESSAGE */
	someip_Message ReadsomeipMessage(boost::asio::ip::tcp::socket & socket) 
	{
       boost::system::error_code error;
       boost::asio::streambuf buf;
       auto bytes = boost::asio::read(socket, buf, boost::asio::transfer_all(),error);
       ostream oss(&buf);
       std::stringstream ss;
       ss<<oss.rdbuf();
	   someip_Message someipMsg;
	   someipMsg.Deserialize(ss);
	   return someipMsg;
	}

	/* INSERT NEW DESTINATION */
	void AddEndPoint(ServiceID serviceId, EndPoint ep)
	{
		//Dispatcher.emplace(des);
	}

	/* GET DESTINATION */
	EndPoint GetEndPoint(ServiceID serviceId)
	{
		return Dispatcher.at(serviceId);
	}

} // Namespace someip