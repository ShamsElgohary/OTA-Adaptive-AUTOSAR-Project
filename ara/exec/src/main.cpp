

#include "../include/applicationExecutionMgr.hpp"
using namespace ara::exec ;
using namespace std;
int main()
{
    ApplicationExecutionMgr app("/home/loay/Documents/GitHub/OTA-Adaptive-AUTOSAR-Project/ara");
    app.initialize();
    app.run();
    return 0;
}