#include "../include/serviceskeleton.hpp"

using namespace std;

bool stopOfferFlag = false;

namespace ara
{
    namespace com
    {
        namespace skeleton
        {

            string parse(string x, ara::com::InstanceIdentifier instance_id, ara::com::serviceIdentifierType service_id, string required)
            {
                ifstream file(x);
                Json::Value actualJson;
                Json::Reader reader;
                reader.parse(file, actualJson);
                for (int i = 0; i < actualJson["ap_service_instances"]["provided_ap_service_instances"].size(); i++)
                {
                    if (actualJson["ap_service_instances"]["provided_ap_service_instances"][i]["instance_id"] == instance_id && actualJson["ap_service_instances"]["provided_ap_service_instances"][i]["service_id"] == service_id)
                    {
                        return actualJson["ap_service_instances"]["provided_ap_service_instances"][i][required].asString();
                    }
                }
            }

            int parse(string x, ara::com::InstanceIdentifier instance_id, ara::com::serviceIdentifierType service_id)
            {
                ifstream file(x);
                Json::Value actualJson;
                Json::Reader reader;
                reader.parse(file, actualJson);
                for (int i = 0; i < actualJson["ap_service_instances"]["provided_ap_service_instances"].size(); i++)
                {
                    if (actualJson["ap_service_instances"]["provided_ap_service_instances"][i]["instance_id"] == instance_id && actualJson["ap_service_instances"]["provided_ap_service_instances"][i]["service_id"] == service_id)
                    {
                        return actualJson["ap_service_instances"]["provided_ap_service_instances"][i]["port"].asInt();
                    }
                }
            }

            skeletonBase::skeletonBase(string path, ara::com::serviceIdentifierType serviceID, ara::com::InstanceIdentifier instanceID, ara::com::MethodCallProcessingMode mode)
                : instanceID{instanceID}, serviceID{serviceID}, mode{mode}
            {
                ara::iam::IAMGrantQuery IGQ;
                ara::iam::Grant G(serviceID, instanceID, "ComGrant", "Provide");
                grant_result = IGQ.HasGrant(G);
                std::cout << "Result: " << grant_result << std::endl;

                if (grant_result)
                {
                    string ip = parse(path, instanceID, serviceID, "ipv4") + " ";
                    int port = parse(path, instanceID, serviceID);
                    cout << port << endl;
                    this->ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(serviceID, instanceID, ip, port, someip::EndUserType::SERVER);
                }
                else
                {
                    cout << "ACCESS FORBIDDEN !!!!!" << endl;
                }
            }
            void skeletonBase::OfferService()
            {
                if (grant_result)
                {
                    this->ptr2bindingProtocol->OfferService();
                    serve();
                }
                else
                {
                    cout << "ACCESS FORBIDDEN !!!!!" << endl;
                }
            }
            void skeletonBase::serve()
            {
                while (1)
                {
                    if (stopOfferFlag == true)
                    {
                        break;
                    }
                    handleMethod();
                }
            }
            void skeletonBase::StopOfferService()
            {
                this->ptr2bindingProtocol->StopOfferService();
            }
        }
    }
}
