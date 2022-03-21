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
	E_OK,                      // No error occurred
	E_NOT_OK,                  // Unspecified error occurred
	E_UNKOWN_SERVICE,          // Service ID is unknown
	E_UNKNOWN_METHOD,          // Method ID is unknown
	E_NOT_READY,               // SERVICE IS NOT RUNNING
	E_NOT_REACHABLE,           // SERVICE IS NOT REACHABLE
	E_TIME_OUT,                // TIMEOUT OCCURRED
	E_WRONG_PROTOCOLVERSION,   // SOME/IP PROTOCOL VERSION IS NOT SUPPORTED
	E_WRONG_INTERFACEVERSION,  // SERVICE INTERFACE VERSION IS NOT SUPPORTED
	E_MALFORMED_MESSAGE,       // DESERIALIZATION ERROR OCCURRED
	E_WRONGMESSAGETYPE,        // INVALID MESSAGE TYPE
	E_E2E_REPEATED,            // REPEATED E2E CALCULATION ERROR
	E_E2E_WRONG_SEQUNECE,      // WRONG E2E SEQUENCE ERROR
	E_E2E,                     // UNSPECIFIED E2E ERROR
	E_E2E_NOTAVAILABLE,        // E2E IS NOT SUPPORTED
	E_E2E_NO_NEWDATA           // NO NEW DATA E2E CALCULATION PRESENT
};


inline ServiceID getServiceID(MessageID messageID) {
	return messageID >> SERVICE_ID_BITS_COUNT; // GET LAST 16 BITS
}

inline MethodID getMethodID(MessageID messageID) {
	return messageID & (0xFFFFFFFF >> SERVICE_ID_BITS_COUNT); // FIRST 16 BITS
}

inline MessageID MakeMessageID(ServiceID serviceID, MethodID MethodID) {
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

		someipHeader(ServiceID serviceId, MethodID methodId, RequestID requestId, 
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

		private:

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

	    friend class boost::serialization::access;

};


    /////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////  SOMEIP MESSAGE 	/////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////


struct someipMessage
{
	public:	
    /* MEMBERS */
    someip::someipHeader header;
	std::string payload;
    
    /* METHODS */
	someipMessage(someipHeader header, std::stringstream &payload);
	someipMessage();
    ~someipMessage();


	private:
	/* NOT EXPLICITILY CALLED BY USER */
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version){
		ar & this->header;
		ar & this->payload;
	}	
	
	friend class boost::serialization::access;
};


} // END OF NAMESPACE "someip"
