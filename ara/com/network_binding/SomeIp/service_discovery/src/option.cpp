#include "../include/option.hpp"


void ipv4_endpoint_option ::setOptionLength(uint16_t length)
{
    this->length=0x0009;
}

void ipv4_endpoint_option::setOptionType(uint8_t type)
{
    this->type=0x04;
}

void ipv4_endpoint_option::setIPV4Address(uint32_t ipv4_address)  //not known  ((defined by language binding))
{
    this->ipv4_address=ipv4_address;
}

void ipv4_endpoint_option::setTransportProtocol(uint8_t l4_proto)
{

this->l4_proto=0x06; //in case of TCP 

}

void ipv4_endpoint_option::setPortNum(uint16_t port_num)  //not known  ((defined by language binding))
{
   this->port_num=port_num;
}

ipv4_endpoint_option::ipv4_endpoint_option(uint32_t ipv4_address, uint8_t port_num)
{
    this->ipv4_address=ipv4_address;
    this->port_num=port_num;
    this->l4_proto=0x06;
    //OptionCounter+=12;

}

  uint32_t ipv4_endpoint_option::getIPV4_Address(void)
  {
      return ipv4_address;
  }
  uint16_t ipv4_endpoint_option::getport_num(void)
  {
      return port_num;
  }
