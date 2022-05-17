#include "../include/applicationExecutionMgr.hpp"

using namespace ara::exec ;
using namespace std;

int main()
{
    ApplicationExecutionMgr app("/home/shams/Github/OTA-Adaptive-AUTOSAR-Project");
    app.initialize();
    app.run();
    return 0;
}
