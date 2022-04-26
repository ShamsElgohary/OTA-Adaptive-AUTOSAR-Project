#include "../include/serviceProxy.hpp"
using namespace std;

namespace ara
{
    namespace com
    {
        namespace proxy
        {

            ProxyBase::ProxyBase(HandleType handle) : ProxyHandle(handle)
            {
            }
            ara::com::ServiceHandleContainer<ProxyBase::HandleType> ProxyBase::FindService(string path, int serviceID)
            {
                ara::com::ServiceHandleContainer<ProxyBase::HandleType> vectorOfHandles;
                vector<serviceinfo> opVsomeip = ara::com::SomeIpNetworkBinding::FindService_SomeIp(serviceID);

                for (uint8_t i = 0; i < opVsomeip.size(); i++)
                {
                    if (IAM_ACTIVATED)
                    {
                        ara::iam::IAMGrantQuery IGQ;
                        ara::iam::Grant G(serviceID, opVsomeip[i].Instance_ID, "ComGrant", "Require");
                        bool grant_result = IGQ.HasGrant(G);
                        if (!grant_result)
                        {
                            cout << "ACCESS FORBIDDEN !!!!!" << endl;
                            continue;
                        }
                    }

                    ProxyBase::HandleType h;
                    h.serviceID = serviceID;
                    h.InstanceID = opVsomeip[i].Instance_ID;
                    h.ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(1, 1, opVsomeip[i].ipv4_address, opVsomeip[i].port_num, someip::EndUserType::CLIENT);
                    vectorOfHandles.push_back(h);
                }

                return vectorOfHandles;
            }

            ara::com::ServiceHandleContainer<ProxyBase::HandleType> ProxyBase::FindService(string path, int serviceID, ara::com::InstanceIdentifier instanceId)
            {
                ara::com::ServiceHandleContainer<ProxyBase::HandleType> vectorOfHandles;

                if (parse(path, serviceID, instanceId) == "SOME/IP")
                {
                    vector<serviceinfo> opVsomeip = ara::com::SomeIpNetworkBinding::FindService_SomeIp(serviceID, instanceId);
                    for (uint8_t i = 0; i < opVsomeip.size(); i++)
                    {
                        if (IAM_ACTIVATED)
                        {
                            ara::iam::IAMGrantQuery IGQ;
                            ara::iam::Grant G(serviceID, opVsomeip[i].Instance_ID, "ComGrant", "Require");
                            bool grant_result = IGQ.HasGrant(G);
                            if (!grant_result)
                            {
                                cout << "ACCESS FORBIDDEN !!!!!" << endl;
                                continue;
                            }
                        }
                        ProxyBase::HandleType h;
                        h.serviceID = serviceID;
                        h.InstanceID = opVsomeip[i].Instance_ID;
                        h.ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(1, 1, opVsomeip[i].ipv4_address, opVsomeip[i].port_num, someip::EndUserType::CLIENT);
                        vectorOfHandles.push_back(h);
                    }
                }

                return vectorOfHandles;
            }

            string ProxyBase::parse(string x, int service_id, int instance_id)
            {
                ifstream file(x);
                Json::Value actualJson;
                Json::Value actualJson2;
                Json::Value actualJson3;
                Json::Reader reader;
                reader.parse(file, actualJson);
                actualJson2 = actualJson["ap_service_instances"];
                actualJson3 = actualJson2["required_ap_service_instances"];
                for (int i = 0; i < actualJson3.size(); i++)
                {
                    if (actualJson3[i]["instance_id"] == instance_id && actualJson3[i]["service_id"] == service_id)
                    {
                        return actualJson3[i]["type"].asString();
                    }
                }
                return "NOT FOUND";
            }

        }
    }
}
