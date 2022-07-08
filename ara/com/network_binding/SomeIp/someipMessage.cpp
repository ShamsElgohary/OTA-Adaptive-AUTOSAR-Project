#include "someipMessage.hpp"

namespace someip
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP HEADER 	/////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

    someipHeader::someipHeader() 
    {
    messageID = requestID = 0;
    returnCode = ReturnCode::E_OK;
    messageType = MessageType::ERROR;
    }   

	someipHeader::someipHeader(MessageID messageID, RequestID requestID, InterfaceVersion interfaceVersion, 
                            MessageType messageType, ReturnCode returnCode) 
    {
		interfaceVersion = 1;
		this->messageID = messageID;
		this->requestID = requestID;
		this->interfaceVersion = interfaceVersion;
		this->messageType = messageType;
		this->returnCode = returnCode;
	}


	someipHeader::someipHeader(ServiceID serviceId, MethodID methodId, RequestID requestId, 
				MessageType messageType,ReturnCode returnCode)
    {
		this->messageID = someip::MakeMessageID(serviceId, methodId);
		this->requestID = requestID;
		this->interfaceVersion = 1;
		this->messageType = messageType;
		this->returnCode = returnCode;
	}


	someipHeader::someipHeader(ServiceID serviceId, MethodID methodId)
    {
		this->messageID = someip::MakeMessageID(serviceId, methodId);
		this->requestID = 0;
		this->interfaceVersion = 1;
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
		this->messageType = messageType;
	}

	bool someipHeader::isErrorType() const {
		return getMessageType() == MessageType::ERROR;
	}

	void someipHeader::setMessageID(MessageID messageID) {
		this->messageID = messageID;
	}

	void someipHeader::setServiceID(ServiceID serviceID) {
		messageID = someip::MakeMessageID( serviceID, getMethodID() );
	}

	ServiceID someipHeader::getServiceID() const {
		return someip::getServiceID(messageID);
	}

	void someipHeader::setMethodID(MethodID MethodID) {
		messageID = someip::MakeMessageID(getServiceID(), MethodID);
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

	uint32_t someipHeader::getLength() {
        return this->length;
		//return sizeof(someipHeader);
	}

	void someipHeader::setLength(uint32_t length)
	{
		this->length = length;	
	}

	uint8_t someipHeader::getInterfaceVersion()
	{
		return this->interfaceVersion;
	}

	uint8_t someipHeader::getProtocolVersion()
	{
		return this->protocolVersion;
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


	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP MESSAGE 	/////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


	someipMessage::someipMessage(someipHeader header, std::stringstream &payload)
	{
		this->header = header;
		this->payload = payload.str();
        this->header.setLength(this->payload.length());
		//(this->payload) << payload.rdbuf();;	
	}

	someipMessage::someipMessage()
	{
		someipHeader s;
		this->header = s;
		this->payload = ""; //EMPTY PAYLOAD
		/* DEFAULT CONSTRUCTOR */
	}


	someipMessage::~someipMessage()
	{
		/* DEFAULT DESTRUCTOR */
	}
	

} // Namespace someip


