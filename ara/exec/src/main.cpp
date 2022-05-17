#include "../include/applicationExecutionMgr.hpp"

using namespace ara::exec ;
using namespace std;

int main()
{
    ApplicationExecutionMgr app("/home/tabakh/Desktop/GP/src/OTA-Adaptive-AUTOSAR-Project/");
    app.initialize();
    app.run();
    return 0;
}
