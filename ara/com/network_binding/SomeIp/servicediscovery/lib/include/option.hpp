#pragma once

#include <stdio.h>
#include <cstdint>

#include <vector>
#include <memory>
#include <iostream>

using namespace std ;

//uint32_t OptionCounter ;
class option
{
    protected:
    uint16_t length;
    uint8_t type;
    public:
    virtual void setOptionLength(uint16_t length)=0;
    virtual void setOptionType(uint8_t type)=0;


    //setters,getters and operator overloading
};
class ipv4_endpoint_option:public option
{  //reserved???
  private:
  string ipv4_address ;
  uint8_t l4_proto;
  uint16_t port_num;
  public:
  ipv4_endpoint_option(string ipv4_address , uint16_t port_num);
  void setOptionLength(uint16_t length);
  void setOptionType(uint8_t type);
  void setIPV4Address(string ipv4_address );
  void setTransportProtocol(uint8_t l4_proto);
  void setPortNum(uint16_t port_num);
  string getIPV4_Address(void);
  uint16_t getport_num(void);
  //setters,getters and operator overloading

  
};