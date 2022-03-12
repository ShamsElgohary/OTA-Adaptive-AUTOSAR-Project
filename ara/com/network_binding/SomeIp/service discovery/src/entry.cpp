#include "../include/entry.hpp"


uint8_t findtype=0x00;
uint8_t offertype=0x01;
//added
service_entry::service_entry(uint16_t Service_ID,uint16_t Method_ID)
{
    counter+=16;
    Service_ID=Service_ID;
    Method_ID=Method_ID;
}
void service_entry::setType(uint8_t type)
{

this->type=type;

}

void service_entry::setIndexFirstOption(uint8_t index_1st_options)
{
 this->index_1st_options=index_1st_options;

}

void service_entry::setIndexSecondOption(uint8_t index_2nd_options)
{
 this->index_2nd_options=index_2nd_options;

}
void service_entry::setMajorVersion(uint16_t major_version)
{
this->major_version=major_version;

}
void service_entry::setFirstOption(uint8_t no_1st_options){

this->no_1st_options=no_1st_options;

}


void service_entry::setSecondOption(uint8_t no_2nd_options){


    this->no_2nd_options=no_2nd_options;
}

void service_entry::setTimeToLive(uint16_t ttl){
    this->ttl=ttl;

}

void service_entry::setServiceID(uint16_t Service_ID){
    this->Service_ID=Service_ID;
}

void service_entry::setInstanceID(uint16_t Instance_ID){
   this->Instance_ID=Instance_ID;
}

void service_entry ::setMinorVersion(uint16_t minor_version){
    this->minor_version=minor_version;
}

service_entry& service_entry:: create_find_service_entry(uint16_t Service_ID,uint16_t Instance_ID)
{   //not sure for return type
   service_entry SERVICE1(Service_ID,Instance_ID);
   SERVICE1.setType(findtype);
   SERVICE1.setServiceID(Service_ID);
   SERVICE1.setInstanceID(Instance_ID);
   SERVICE1.setMajorVersion(0xFF);
   SERVICE1.setMinorVersion(0xFFFFFFFF);
   SERVICE1.setTimeToLive(0xFFFFFF);
   return SERVICE1;

}
//return instance wla y-return pointer 3al instence ??

service_entry& service_entry:: create_offer_service_entry(uint16_t Service_ID,uint16_t Instance_ID)
{   //not sure for return type
   service_entry SERVICE1(Service_ID,Instance_ID);
   SERVICE1.setType(offertype);
   SERVICE1.setServiceID(Service_ID);
   SERVICE1.setInstanceID(Instance_ID);
   SERVICE1.setMajorVersion(0xFF);
   SERVICE1.setMinorVersion(0xFFFFFFFF);
   SERVICE1.setTimeToLive(0xFFFFFF);
   return SERVICE1;
}

service_entry& service_entry:: create_stop_offer_service_entry(uint16_t Service_ID,uint16_t Instance_ID)
{   //not sure for return type
   service_entry SERVICE1(Service_ID,Instance_ID);
   SERVICE1.setType(offertype);
   SERVICE1.setServiceID(Service_ID);
   SERVICE1.setInstanceID(Instance_ID);
   SERVICE1.setMajorVersion(0xFF);
   SERVICE1.setMinorVersion(0xFFFFFFFF);
   SERVICE1.setTimeToLive(0x000000);
   return SERVICE1;

}

uint8_t service_entry::getType(void)
{
    return type;
}


uint16_t service_entry::getTimeToLIve(void)
{
    return ttl;
}

uint16_t service_entry::getServiceID(void)
{

    return Service_ID;
}

uint16_t service_entry::getInstanceID(void)
{
return Instance_ID;
}