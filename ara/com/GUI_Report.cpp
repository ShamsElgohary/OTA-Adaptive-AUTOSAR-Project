#include "include/GUI_Report.hpp"

using namespace std;
namespace ara
{
    namespace com
    {
        void AddMethodCall(int methodID, string methodName, MethodType Type, uint16_t ServiceID, string Cluster_Name)
        {
            // Test
            string Typestr = (Type == MethodType::Skeleton_Method) ? "Skeleton" : "Proxy";
            cout << "[GUI REPORT] Method ID: " << methodID << ", Method Name: " << methodName << ", Method Type: " << Typestr << endl;

            // Read JSON File
            Json::Value event;
            string file_name = "GUI_Report.json";
            string cluster_name = Cluster_Name;
            ifstream f(file_name);
            Json::Reader R;
            R.parse(f, event);
            if (!event)
            {
                event["Cluster_name"] = cluster_name;
                event["com_json"]["Service_Discovery_Request"] = Json::arrayValue;
                event["com_json"]["Skeleton_Provided_Method"] = Json::arrayValue;
                event["com_json"]["Proxy_Required_Method"] = Json::arrayValue;
            }

            // Add To JSON FILE
            Json::Value Temp;
            Temp["Method_ID"] = methodID;
            Temp["Method_Name"] = methodName;
            Temp["Service_ID"] = ServiceID;

            if (Type == MethodType::Skeleton_Method)
            {
                event["com_json"]["Skeleton_Provided_Method"].append(Temp);
            }
            else if (Type == MethodType::Proxy_Method)
            {
                event["com_json"]["Proxy_Required_Method"].append(Temp);
            }

            // std::cout << event << std::endl;
            std::ofstream json_file(file_name);
            json_file << event;
            json_file.close();
            if (SIMULATION_ACTIVE)
            {
                // Report
                // simulation s(8080);
                // s.connect_to_socket();
                // s.send_file(file_name);
            }
        }

        void AddServiceDiscoveryRequest(uint16_t ServiceID, uint16_t InstanceID, ServiceDiscoveryMethodType Type, bool GrantResult, string Cluster_Name)
        {
            // Test
            string Typestr = "";
            if (Type == ServiceDiscoveryMethodType::Find_Method)
            {
                Typestr = "Find";
            }
            else if (Type == ServiceDiscoveryMethodType::Offer_Method)
            {
                Typestr = "Offer";
            }
            else if (Type == ServiceDiscoveryMethodType::Stop_Method)
            {
                Typestr = "Stop";
            }
            cout << "[GUI REPORT] Service ID: " << ServiceID << ", Instance ID: " << InstanceID << ", Method Type: " << Typestr << ", Grant Result: " << GrantResult << endl;

            // Read JSON File
            Json::Value event;
            string file_name = "GUI_Report.json";
            string cluster_name = Cluster_Name;
            ifstream f(file_name);
            Json::Reader R;
            R.parse(f, event);
            if (!event)
            {
                event["Cluster_name"] = cluster_name;
                event["com_json"]["Service_Discovery_Request"] = Json::arrayValue;
                event["com_json"]["Skeleton_Provided_Method"] = Json::arrayValue;
                event["com_json"]["Proxy_Required_Method"] = Json::arrayValue;
            }

            // Add To JSON FILE
            Json::Value Temp;
            Temp["Service_ID"] = ServiceID;
            Temp["Instance_ID"] = InstanceID;
            Temp["Type"] = Typestr;
            Temp["Grant_Result"] = GrantResult;

            event["com_json"]["Service_Discovery_Request"].append(Temp);

            // std::cout << event << std::endl;
            std::ofstream json_file(file_name);
            json_file << event;
            json_file.close();

            if (SIMULATION_ACTIVE)
            {
                // Report
                // simulation s(8080);
                // s.connect_to_socket();
                // s.send_file(file_name);
            }
        }
    }
}