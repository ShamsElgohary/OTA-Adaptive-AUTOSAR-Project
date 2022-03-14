#pragma once

#include <string>
#include <sstream>
#include <bits/stdc++.h>
#include <iostream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "Serialization.hpp"


typedef uint32_t MessageID;  
typedef uint32_t RequestID;
typedef uint16_t ServiceID;
typedef uint16_t MethodID;
typedef uint16_t InstanceID;
typedef uint16_t EventGroupID;
typedef uint8_t ProtocolVersion;
typedef uint8_t InterfaceVersion;


namespace someip{

static constexpr InstanceID ANY_INSTANCE_ID = 0xFFFF;
static constexpr InstanceID ANY_SERVICE_ID = 0xFFFF;
static const int SERVICE_ID_BITS_COUNT = 16;


// SOME/IP MESSAGE TYPE
enum MessageType 
{
	REQUEST = 0x00,           // Request expecting a response
	REQUEST_NO_RETURN = 0x01,   // Fire and forget request
	NOTIFICATION = 0x02,      // Notification/event callback
	RESPONSE = 0x80,          // Response without any error
	ERROR = 0x81,             // Response containing error
	TP_REQUEST = 0x20,         // Transfer protocol request
	TP_REQUEST_NO_RETURN = 0x21, // Transfer protocol fire and forget request
	TP_NOTIFICATION = 0x22,    // Transfer protocol notification
	TP_RESPONSE = 0xa0,        // Transfer protocol response
	TP_ERROR = 0xa1            // Transfer protocol error
};



// SOME/IP MESSAGE RETURN CODE
enum ReturnCode
{
	E_OK,                   // No error occurred
	E_NOT_OK,               // Unspecified error occurred
	E_UNKOWN_SERVICE,       // Service ID is unknown
	E_UNKNOWN_METHOD,       // Method ID is unknown
	eNotReady,              // Service is not running
	eNotReachable,          // Service is not reachable
	eTimeout,               // Timeout occurred
	eWrongProtocolVersion,  // SOME/IP protocol version is not supported
	eWrongInterfaceVersion, // Service interface version is not supported
	eMalformedMessage,      // Deserialization error occurred
	eWrongMessageType,      // Invalid message type
	eE2eRepeated,           // Repeated E2E calculation error
	eE2eWrongSequnece,      // Wrong E2E sequence error
	eE2e,                   // Unspecified E2E error
	eE2eNotAvailable,       // E2E is not supported
	eE2eNoNewData           // No new data E2E calculation present
};


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


	/////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP HEADER 	/////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


class someipHeader {

	private:
		MessageID messageID;
		uint32_t  length;
		RequestID requestID;
		ProtocolVersion protocolVersion = 1;
		InterfaceVersion interfaceVersion ;
		MessageType messageType;
		ReturnCode returnCode;

	public:
		someipHeader();
		someipHeader(MessageID messageID, RequestID requestID, InterfaceVersion interfaceVersion, 
				MessageType messageType,ReturnCode returnCode);
		
		MessageID getMessageID() const;
		MethodID getMethodID() const;
		MessageType getMessageType() const;
		RequestID getRequestID() const;
		ServiceID getServiceID() const;
		uint32_t getLength();


		void setMessageType(MessageType messageType);
		void setMessageID(MessageID messageID);
		void setServiceID(ServiceID serviceID);
		void setMethodID(MethodID MethodID);
		void setRequestID(RequestID id);
		void setLength(uint32_t length);


		bool isReply() const ;
		bool isErrorType() const;
		bool isNotification() const;
		bool isRequestWithReturn() const;

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
};


    /////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP MESSAGE 	/////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


struct someip_Message
{	
    /* MEMBERS */
    someip::someipHeader header;
	std::string payload;
    
    /* METHODS */
	someip_Message(someipHeader header, std::stringstream &payload);
	someip_Message();
    ~someip_Message();
	
    /* USER CAN CALL THIS SERIALIZE (NAME IS STANDARDIZED AS LIKE ALL OTHER METHODS) */
	void Serialize(std::stringstream &ss);
	void Deserialize(std::stringstream &ss);

	/* NOT EXPLICITILY CALLED BY USER */
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version){
		ar & this->header;
		ar & this->payload;
	}	
};


} // END OF NAMESPACE "someip"
