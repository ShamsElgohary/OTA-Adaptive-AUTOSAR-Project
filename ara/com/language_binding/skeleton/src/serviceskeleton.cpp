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
                cout << "[com::Skeleton::Parse] Couldn't Find Data from Instance Manifest" << endl;
                return "";
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
                cout << "[com::skeleton::Parse] Couldn't Find Data from Instance Manifest" << endl;
                return -1;
            }

            skeletonBase::skeletonBase(string path, ara::com::serviceIdentifierType serviceID, ara::com::InstanceIdentifier instanceID, ara::com::MethodCallProcessingMode mode)
                : instanceID{instanceID}, serviceID{serviceID}, mode{mode}
            {
                if (IAM_ACTIVATED)
                {
                    ara::iam::IAMGrantQuery IGQ;
                    ara::iam::Grant G(serviceID, instanceID, "ComGrant", "Provide");
                    grant_result = IGQ.HasGrant(G);
                    if (!grant_result)
                    {
                        cout << "[com::skeleton::skeletonBase] ACCESS FORBIDDEN !!!!!" << endl;
                        return;
                    }
                }

                string ip = parse(path, instanceID, serviceID, "ipv4") + " ";
                int port = parse(path, instanceID, serviceID);
                cout << "IP Address: " << ip << endl;
                cout << "Port: " << port << endl;
                this->ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(serviceID, instanceID, ip, port, someip::EndUserType::SERVER);
            }
            void skeletonBase::OfferService()
            {
                if (grant_result || IAM_ACTIVATED)
                {
                    this->ptr2bindingProtocol->OfferService();
                    serve();
                }
                else
                {
                    cout << "[com::skeleton::OfferService] ACCESS FORBIDDEN !!!!!" << endl;
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
