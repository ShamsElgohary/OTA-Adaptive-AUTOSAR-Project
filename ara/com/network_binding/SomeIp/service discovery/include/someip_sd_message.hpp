#pragma once
#include <stdio.h>
#include <cstdint>
#include <vector>
#include <memory>
#include "../include/entry.hpp"
#include "../include/option.hpp"
#include "../../SOMEIP.hpp"


enum MessageType {
	REQUEST = 0x00, REQUEST_NO_RETURN = 0x01, NOTIFICATION = 0x02, RESPONSE = 0x80, ERROR = 0x81,
};


Flag flag=0xE0;
EntryLength entryLength=0x00000010;


class someip_sd_message : public someip::someip_Message  
// someip_sd_message will have its payload and header
// someip_sd_message can use send function
{
    private:
    //someipheader someip_header; //not implemented
    uint8_t flags;
    uint32_t entries_length;
    uint32_t options_length;
    std::vector<std::unique_ptr<entry>>entries;   //vector of unique pointers 3la class service_entry
    std::vector <std::unique_ptr<ipv4_endpoint_option>>options;
    public:
    someip_sd_message();
    std::unique_ptr <entry>GetEntry(void);
    std::unique_ptr <ipv4_endpoint_option>GetOption(void);
    void AddEntry(std::unique_ptr<entry> s1);
    void AddOption(std::unique_ptr<ipv4_endpoint_option> Option1);
    //set option length
    //someip_sd_message(ServiceID serviceID,MethodID methodID,ClientID clientID,SessionID sessionID,ProtocolVersion protocol_version,MessageType messageType,ReturnCode returnCode);
    void setflag(Flag flag);
    void setEntryLength(uint32_t counter);
    void setOptionsLength(uint32_t OptionCounter);

};

