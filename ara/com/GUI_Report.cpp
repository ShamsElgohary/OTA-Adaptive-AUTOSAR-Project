#include "include/GUI_Report.hpp"

using namespace std;
namespace ara
{
    namespace com
    {
        void AddMethodCall(int methodID, string methodName, MethodType Type, string Exe_Name, uint16_t ServiceID)
        {
            // Test
            string Typestr = (Type == MethodType::Skeleton_Method) ? "Skeleton" : "Proxy";
            cout << "[GUI REPORT] Method ID: " << methodID << ", Method Name: " << methodName << ", Method Type: " << Typestr << endl;
            // Read JSON File
            Json::Value event;
            ifstream f("com.json");
            Json::Reader R;
            R.parse(f, event);
            if (!event)
            {
                event["Cluster_name"] = "COM_" + Exe_Name;
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

            std::cout << event << std::endl;
            std::ofstream json_file("com.json");
            json_file << event;
            json_file.close();

            // Report
            simulation s(8080);
            s.connect_to_socket();
            s.send_file("com.json");
        }

        void AddServiceDiscoveryRequest(uint16_t ServiceID, uint16_t InstanceID, ServiceDiscoveryMethodType Type, bool GrantResult, string Exe_Name)
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

            ifstream f("com.json");
            Json::Reader R;
            R.parse(f, event);
            if (!event)
            {
                event["Cluster_name"] = "COM_" + Exe_Name;
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

            std::cout << event << std::endl;
            std::ofstream json_file("com.json");
            json_file << event;
            json_file.close();

            // Report
            simulation s(8080);
            s.connect_to_socket();
            s.send_file("com.json");
        }
    }
}
