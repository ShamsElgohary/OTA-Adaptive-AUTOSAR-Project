#include "../include/serviceProxy.hpp"
using namespace std;

namespace ara
{
    namespace com
    {
        namespace proxy
        {

            ProxyBase::ProxyBase(HandleType handle) : ProxyHandle(handle) {}
            ara::com::ServiceHandleContainer<ProxyBase::HandleType> ProxyBase::FindService(int serviceID)
            {
                ara::com::ServiceHandleContainer<ProxyBase::HandleType> vectorOfHandles;


                vector<serviceinfo> opVsomeip = ara::com::SomeIpNetworkBinding::FindService_SomeIp(serviceID);
                
                for (uint8_t i = 0; i < opVsomeip.size(); i++)
                {
                    ProxyBase::HandleType h;
                    h.InstanceID = opVsomeip[i].Instance_ID;
                    h.ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(opVsomeip[i].ipv4_address, opVsomeip[i].port_num);
                    vectorOfHandles.push_back(h);
                }

                return vectorOfHandles;
            }

            ara::com::ServiceHandleContainer<ProxyBase::HandleType> ProxyBase::FindService(int serviceID, ara::com::InstanceIdentifier instanceId)
            {
                ara::com::ServiceHandleContainer<ProxyBase::HandleType> vectorOfHandles;
                if (1) //if someip
                {
                    vector<serviceinfo> opVsomeip = ara::com::SomeIpNetworkBinding::FindService_SomeIp(serviceID ,instanceId);
                    for (uint8_t i = 0; i < opVsomeip.size(); i++)
                    {
                        ProxyBase::HandleType h;
                        h.InstanceID = opVsomeip[i].Instance_ID;
                        h.ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(opVsomeip[i].ipv4_address, opVsomeip[i].port_num);
                        vectorOfHandles.push_back(h);
                    }
                    return vectorOfHandles;
                }
            }

        }
    }
}
