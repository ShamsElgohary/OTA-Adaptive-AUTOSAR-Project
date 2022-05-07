#include "../include/applicationExecutionMgr.hpp"

using namespace ara::exec ;
using namespace std;

int main()
{
    ApplicationExecutionMgr app("/home/yasmin/Desktop/Graduation_Project/02-OurImpement/OTA-Adaptive-AUTOSAR-Project");
    app.initialize();
    app.run();
    return 0;
}
