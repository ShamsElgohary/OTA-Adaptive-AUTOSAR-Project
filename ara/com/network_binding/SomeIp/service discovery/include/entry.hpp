#pragma once

#include <stdio.h>
#include <cstdint>

#include <vector>
#include <memory>

//static uint32_t counter;

class entry
{
protected:
uint8_t type;
uint8_t index_1st_options; //unique pointer
uint8_t index_2nd_options;
uint8_t no_1st_options;
uint8_t no_2nd_options;
uint16_t major_version;
uint32_t ttl;
uint16_t Service_ID;
uint16_t Instance_ID;
entry();
public:
virtual void setType (uint8_t type)=0;
virtual void setIndexFirstOption(uint8_t index_1st_options)=0;
virtual void setIndexSecondOption(uint8_t index_2nd_options)=0;
virtual void setMajorVersion(uint16_t major_version)=0;
virtual void setFirstOption(uint8_t no_1st_options)=0;
virtual void setSecondOption(uint8_t no_2nd_options)=0;
virtual void setTimeToLive(uint32_t ttl)=0;
virtual uint32_t getTimeToLIve(void)=0;
virtual void setServiceID(uint16_t Service_ID)=0;
virtual void setInstanceID(uint16_t Instance_ID)=0;
virtual uint8_t getType(void)=0;
virtual uint16_t getServiceID(void)=0;
virtual uint16_t getInstanceID(void)=0;

};

class service_entry:public entry
{
    private:
    uint16_t minor_version;
    

    public:
    service_entry();
    service_entry(uint16_t serviceID,uint16_t InstanceID);  //check
     void setType (uint8_t type);
     void setIndexFirstOption(uint8_t index_1st_options);
     void setIndexSecondOption(uint8_t index_2nd_options);
     void setMajorVersion(uint16_t major_version);
     void setFirstOption(uint8_t no_1st_options);
     void setSecondOption(uint8_t no_2nd_options);
     void setTimeToLive(uint32_t ttl);
     void setServiceID(uint16_t Service_ID);
     void setInstanceID(uint16_t Instance_ID);
     void setMinorVersion(uint16_t minor_version);
     uint16_t getServiceID(void);
     uint16_t getInstanceID(void);
     uint32_t getTimeToLIve(void);
     uint8_t getType(void);
    static service_entry *create_find_service_entry(uint16_t Service_ID,uint16_t Instance_ID);  //changeddd
    static service_entry * create_offer_service_entry(uint16_t Service_ID,uint16_t Instance_ID);
    static service_entry * create_stop_offer_service_entry(uint16_t Service_ID,uint16_t Instance_ID);
};