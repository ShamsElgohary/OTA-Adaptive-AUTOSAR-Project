#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <string>
#include "NetworkBinding.hpp"
#include "../../core/types.hpp"

using namespace std;

namespace ara
{
    namespace com
    {


        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////  NETWORKBASE CLASS  ////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////

        /* InstanceId: "SOME/IP 1" */
        BindingProtocol NetworkBindingBase::GetType(std::string instanceIdStr, uint16_t &instanceId)
        {
            std::string strBindingDDS = "DDS";
            /* SOME/IP */
            std::string strBindingSOMEIP = "SOME";
            std::string instanceIdDigits{""};
            BindingProtocol bindType;

            /* FIND SOME/IP */
            if(instanceIdStr.find(strBindingSOMEIP) )
            {
                for(uint8_t i = 0; i < instanceIdStr.size() ; i++)
                {
                    while( (instanceIdStr[i] >= '0') && (instanceIdStr[i] >= '9') )
                    { 
                        instanceIdDigits += instanceIdStr[i];
                    }
                }

                if(instanceIdDigits != "")
                {
                    instanceId = stoi(instanceIdDigits);
                }                

                bindType = BindingProtocol::SOMEIP;
            }

            /* FIND DDS */
            else if(instanceIdStr.find(strBindingDDS) )
            {
                bindType = BindingProtocol::DDS;
            }
        
            return bindType;
        }


        std::shared_ptr<NetworkBindingBase> NetworkBindingBase::Bind(uint16_t serviceId, std::string instanceIdStr, uint16_t port)
        {
            uint16_t instanceId;
            BindingProtocol bindType = NetworkBindingBase::GetType(instanceIdStr, instanceId);
            
            std::shared_ptr<NetworkBindingBase> networkBasePtr;
            if (bindType == BindingProtocol::SOMEIP)
            {
                /* INSTANCE ID IN SOMEIP IS OF TYPE UINT16_T */
                networkBasePtr = std::make_shared<SomeIpNetworkBinding> (serviceId,instanceId ,port);
            }

            else if (bindType == BindingProtocol::DDS)
            {
                /* INSTANCE ID IN DDS IS A STRING */
                networkBasePtr = std::make_shared<DDSNetworkBinding> (serviceId,instanceIdStr ,port);
            }

            return networkBasePtr;
        }
       



        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// SOMEIP NETWORK BINDING ///////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////

        SomeIpNetworkBinding::SomeIpNetworkBinding(uint16_t serviceId, uint16_t instanceId, uint16_t port,uint32_t ipv4_address)
        {
            this->serviceId  = serviceId;
            this->instanceId = instanceId;
            this->port = port;
            this->ipv4_address=ipv4_address;
        }

        void SomeIpNetworkBinding::Send()
        {
       

        }

        void SomeIpNetworkBinding::Receive()
        {


        }

        void SomeIpNetworkBinding::SendRequest()
        {


        }

        void SomeIpNetworkBinding::OfferService()
        {
        servicediscovery sd;
        sd.offer_service(this->serviceId,this->instanceId,this->port,this->port);
        }
        void SomeIpNetworkBinding::StopOfferService()
        {
         servicediscovery sd;
         sd.stop_offer_service(this->serviceId,this->instanceId);
        }



        
        /////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////// DDS NETWORK BINDING //////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////

        DDSNetworkBinding::DDSNetworkBinding(uint16_t serviceId, std::string instanceIdStr, uint16_t port)
        {
            this->serviceId  = serviceId;
            this->instanceIdStr = instanceIdStr;
            this->port = port;
        }

        void DDSNetworkBinding::Send()
        {

        }

        void DDSNetworkBinding::Receive()
        {

        }

        void DDSNetworkBinding::SendRequest()
        {

        }

        void DDSNetworkBinding::OfferService()
        {

        }

        void DDSNetworkBinding::OfferService(std::string instanceId)
        {


        }

        void DDSNetworkBinding::StopOfferService(std::string instanceId)
        {


        }

    }
}

