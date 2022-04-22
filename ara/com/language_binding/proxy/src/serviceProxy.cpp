#include "../include/serviceProxy.hpp"
using namespace std;

namespace ara
{
    namespace com
    {
        namespace proxy
        {
            string parse(string x, int service_id);

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

                if (parse("service_manifest.json", instanceId) == "SOME/IP")
                {
                    vector<serviceinfo> opVsomeip = ara::com::SomeIpNetworkBinding::FindService_SomeIp(serviceID, instanceId);
                    for (uint8_t i = 0; i < opVsomeip.size(); i++)
                    {
                        ProxyBase::HandleType h;
                        h.InstanceID = opVsomeip[i].Instance_ID;
                        h.ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(opVsomeip[i].ipv4_address, opVsomeip[i].port_num);
                        vectorOfHandles.push_back(h);
                    }
                    
                }
                
                return vectorOfHandles;
            }

            string parse(string x, int instance_id)
            {
                ifstream file(x);
                Json::Value actualJson;
                Json::Value actualJson2;
                Json::Value actualJson3;
                Json::Reader reader;
                reader.parse(file, actualJson);
                actualJson2 = actualJson["ap_service_instances"];
                actualJson3 = actualJson2["provided_ap_service_instances"];
                for (int i = 0; i < actualJson3.size(); i++)
                {
                    if (actualJson3[i]["instance_id"] == instance_id)
                    {
                        return actualJson3[i]["type"].asString();
                    }
                }
                return "NOT FOUND";
            }

        }
    }
}
