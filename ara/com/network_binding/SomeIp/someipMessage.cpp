#include "someipMessage.hpp"

namespace someip
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP HEADER 	/////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

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
		this->messageID = messageID;
		this->requestID = requestID;
		this->interfaceVersion = interfaceVersion;
		this->messageType = messageType;
		this->returnCode = returnCode;
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

	uint32_t someipHeader::getLength() {
        return this->length;
		//return sizeof(someipHeader);
	}

	void someipHeader::setLength(uint32_t length)
	{
		this->length = length;	
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


	someip_Message::someip_Message(someipHeader header, std::stringstream &payload)
	{
		this->header = header;
		this->payload = payload.str();
        this->header.setLength(this->payload.length());
		//(this->payload) << payload.rdbuf();;	
	}

	someip_Message::someip_Message()
	{
		/* DEFAULT CONSTRUCTOR */
	}


	someip_Message::~someip_Message()
	{
		/* DEFAULT DESTRUCTOR */
	}
	
	/* USER CAN CALL THIS SERIALIZE (NAME IS STANDARDIZED AS LIKE ALL OTHER METHODS) */
	void someip_Message::Serialize(std::stringstream &ss){
		/* SERIALIZTION OF STRUCT */
		boost::archive::text_oarchive archive(ss);
		archive << *this;
	}

	void someip_Message::Deserialize(std::stringstream &ss){
		/* DESERIALIAZTION OF STRUCT */
		std::istringstream iss(ss.str());
		boost::archive::text_iarchive ia(iss);
		ia >> *this;
	}





} // Namespace someip


