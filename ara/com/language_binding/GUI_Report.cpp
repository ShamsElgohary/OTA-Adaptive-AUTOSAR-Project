#include "../include/GUI_Report.hpp"

using namespace std;
namespace ara
{
    namespace com
    {
        void AddMethodCall(int methodID, string methodName, MethodType Type)
        {
            // Test 
            string Typestr = (Type == MethodType::Skeleton_Method) ? "Skeleton" : "Proxy";
            cout << "[GUI REPORT] Method ID: " << methodID << ", Method Name: " << methodName << ", Method Type: " << Typestr << endl;
            // Read JSON File
            
            // Add To JSON FILE

            // Report
        }

        void AddServiceDiscoveryRequest(uint16_t ServiceID, uint16_t InstanceID, ServiceDiscoveryMethodType Type, bool GrantResult)
        {
            // Test 
            string Typestr;
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
            cout << "[GUI REPORT] Service ID: " << ServiceID << ", Instance ID: " << InstanceID << ", Method Type: " << Typestr << ", Grant Result: " << GrantResult <<  endl;
            // Read JSON File

            // Add To JSON FILE

            // Report
        }
    }
}
