#pragma once

#include <string>
#include <sstream>
#include <bits/stdc++.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <functional>
#include "Serialization.hpp"
#include "stdint.h"


namespace someip {
				
typedef uint32_t MessageID;  /* SERVICEID + METHODID */
typedef uint16_t ServiceID;
typedef uint16_t MethodID;
typedef uint16_t InstanceID;
typedef uint16_t EventGroupID;
typedef uint32_t RequestID;
typedef uint8_t ProtocolVersion;
typedef uint8_t InterfaceVersion;

static constexpr InstanceID ANY_INSTANCE_ID = 0xFFFF;
static constexpr InstanceID ANY_SERVICE_ID = 0xFFFF;
static const int SERVICE_ID_BITS_COUNT = 16;


enum MessageType {
	REQUEST = 0x00, REQUEST_NO_RETURN = 0x01, NOTIFICATION = 0x02, RESPONSE = 0x80, ERROR = 0x81,
};


enum ReturnCode {
	E_OK,
};


struct someipHeader {

	public:
		someipHeader();
		someipHeader(MessageID messageID, RequestID requestID, InterfaceVersion interfaceVersion, 
				MessageType messageType,ReturnCode returnCode);
		
		MessageID getMessageID() const;
		MethodID getMethodID() const;
		MessageType getMessageType() const;
		void setMessageType(MessageType messageType);
		bool isErrorType() const;
		void setMessageID(MessageID messageID);
		void setServiceID(ServiceID serviceID);
		ServiceID getServiceID() const;
		void setMethodID(MethodID MethodID);
		bool isReply() const ;
		bool isNotification() const;
		bool isRequestWithReturn() const;
		void setRequestID(RequestID id);
		RequestID getRequestID() const;
		static size_t getLength();
		bool operator==(const someipHeader& right) const;
		someipHeader& operator=(const someipHeader& right);

		/* NOT EXPLICITILY CALLED BY USER */
		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version){
			ar & messageID;
			ar & length;
			ar & requestID;
			ar & protocolVersion;
			ar & interfaceVersion;
			ar & messageType;
			ar & returnCode;
		}

	private:
		MessageID messageID;
		uint32_t  length;
		RequestID requestID;
		ProtocolVersion protocolVersion = 1;
		InterfaceVersion interfaceVersion;
		MessageType messageType;
		ReturnCode returnCode;
};

struct someip_Message
{	
	someip_Message(someipHeader header, std::stringstream &payload)
	{
		this->header = header;
		this->payload = payload.str();
		//(this->payload) << payload.rdbuf();;	
	}

	someip_Message()
	{

	}

	/* NOT EXPLICITILY CALLED BY USER */
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version){
		ar & this->header;
		ar & this->payload;
	}
	
	/* USER CAN CALL THIS SERIALIZE (NAME IS STANDARDIZED AS LIKE ALL OTHER METHODS) */
	void Serialize(std::stringstream &ss){
		/* SERIALIZTION OF STRUCT */
		boost::archive::text_oarchive archive(ss);
		archive << *this;
	}

	void Deserialize(std::stringstream &ss){
		/* DESERIALIAZTION OF STRUCT */
		std::istringstream iss(ss.str());
		boost::archive::text_iarchive ia(iss);
		ia >> *this;
	}

	someip::someipHeader header;
	std::string payload;
};

typedef struct 
{
	std::string raw_ip_address;
	unsigned short port_num;
}EndPoint;

inline ServiceID getServiceID(MessageID messageID) {
	return messageID >> SERVICE_ID_BITS_COUNT; // GET LAST 16 BITS
}

inline MethodID getMethodID(MessageID messageID) {
	return messageID & (0xFFFFFFFF >> SERVICE_ID_BITS_COUNT); // FIRST 16 BITS
}

inline MessageID getMessageID(ServiceID serviceID, MethodID MethodID) {
	MessageID messageID = serviceID;
	messageID = (messageID << SERVICE_ID_BITS_COUNT) + MethodID;
	return messageID;
}

/* FUNCTION TO SEND A someip MESSAGE */
void SendsomeipMessage(someip_Message &msg, EndPoint ep);

/* FUNCTION TO RECEIVE A someip MESSAGE */
someip_Message ReadsomeipMessage(boost::asio::ip::tcp::socket & socket);

/* ROUTING */
static std::map <ServiceID , EndPoint > Dispatcher;

/* INSERT NEW ENDPOINT */
void AddEndPoint(ServiceID serviceId, EndPoint ep);

/* GET ENDPOINT */
EndPoint GetEndPoint(ServiceID serviceId);


} // End of Namespace someip

