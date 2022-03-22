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

            skeletonBase::skeletonBase(ara::com::serviceIdentifierType serviceID, ara::com::InstanceIdentifier instanceId, ara::com::MethodCallProcessingMode mode)
                : instanceID{instanceID}, serviceID{serviceID}, mode{mode}
            {
                //parsing to get ip and port type of network binding
                string ip = parse("service_manifest.json", instanceId, serviceID, "ipv4");
                int port = parse("service_manifest.json", instanceId, serviceID);

                this->ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(serviceID, instanceID, ip, port);
            }
            void skeletonBase::OfferService()
            {
                this->ptr2bindingProtocol->OfferService();
                serve();
            }
            void skeletonBase::serve()
            {
                while (1)
                {
                    if (stopOfferFlag == true)
                    {
                        break;
                    }
                    handleMethod(this->ptr2bindingProtocol->get_method_id());
                }
            }
            void skeletonBase::StopOfferService()
            {
                this->ptr2bindingProtocol->StopOfferService();
            }
        }
    }
}

// to be generated
// void Serviceskeleton::handleMethod(int id)
// {
//     switch (id)
//     {
//     case 1:
//         input_add in;
//         this->ptr2bindingProtocol->receive(in.i , in.y);
//         int out = add(in.i, in.y);
//         this->ptr2bindingProtocol->send(out);
//         break;
//     }
// }
