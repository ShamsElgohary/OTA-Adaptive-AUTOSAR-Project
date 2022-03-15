#include <stdio.h>
#include <cstdint>

#include <vector>
#include <memory>
#include "../include/someip_sd_message.hpp"
#include <utility>
#include "entry.cpp"
#include "option.cpp"
//#include "../../SOMEIP.cpp"
//#include"SOMEIP_TCP.cpp"
//#include"SOMEIP_Message.cpp"
#include <iostream>
#include <fstream>
someip_sd_message::someip_sd_message()
{ 
 this->flags=0xE0;
 this->entries_length=0x00000010;
  
}
void someip_sd_message::setflag(Flag flag)
{
    this->flags=flag;
}


/*
void someip_sd_message::setEntryLength(uint32_t counter)
{
   this->entries_length=counter;
}

void someip_sd_message::setOptionsLength(uint32_t OptionCounter)
{
    this->options_length=OptionCounter;
}
*/
uint8_t someip_sd_message::getFlag()
{
    return this->flags;
}

void someip_sd_message::AddEntry(std::unique_ptr<service_entry> &s1)

{
    if(dynamic_cast<service_entry*>(s1.get())!=nullptr) // check type of pointer
    {
    service_entry* se=dynamic_cast<service_entry*>(s1.get()); //cast pointer to ipv4_option_type
    Serializer s;
    std::stringstream ss;
    std::string entryflag="entry";
    s.Serialize(ss,entryflag,se->getServiceID(),se->getInstanceID(),se->getTimeToLIve(),se->getType());
    fstream my_file;
    my_file.open("mariam", std::ios_base::out);
	if (!my_file) {
		cout << "File not created!";
	}
	else {
		cout << "File created successfully!";
      std::string s = ss.str();
      my_file<<s;
		my_file.close(); 
	}
     std::cout<<ss.str();
    this->payload+=ss.str(); //add serialized data to payload of message
    entries.push_back(std::move(s1));
    }

}

void someip_sd_message::AddOption( std::unique_ptr<ipv4_endpoint_option>& Option1)
{
    if(dynamic_cast<ipv4_endpoint_option*>(Option1.get())!=nullptr) // check type of pointer
    {
    ipv4_endpoint_option* op=dynamic_cast<ipv4_endpoint_option*>(Option1.get()); //cast pointer to ipv4_option_type
    Serializer s;
    std::stringstream ss;
    std::string optionflag="option";
    s.Serialize(ss,optionflag,op->getIPV4_Address(),op->getport_num()); // to be edited
    fstream my_file;
    my_file.open("waleed", std::ios_base::out);
	if (!my_file) {
		cout << "File not created!";
	}
	else {
		cout << "File created successfully!";
      std::string s = ss.str();
      my_file<<s;
		my_file.close(); 
	}
    std::cout<<ss.str();
    this->payload+=ss.str(); // add serialized data to payload of message
    options.push_back(std::move(Option1));
    }
}

std::unique_ptr <entry> someip_sd_message::GetEntry(void)
{
    std::unique_ptr<entry> entry1;
    entry1= std::move(entries.back());  
    entries.pop_back();
    return std::move(entry1);
}

std::unique_ptr<ipv4_endpoint_option> someip_sd_message::GetOption(void)
{
    std::unique_ptr<ipv4_endpoint_option> option1;
     option1= std::move(options.back());  
    options.pop_back();
    return std::move(option1);
}


someip_sd_message::~someip_sd_message()
{
    
}