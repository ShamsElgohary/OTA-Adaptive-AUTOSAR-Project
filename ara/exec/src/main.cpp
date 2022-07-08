#include "applicationExecutionMgr.hpp"

using namespace ara::exec ;
using namespace std;

int main()
{
    ApplicationExecutionMgr app(CUSTOMIZED_PROJECT_PATH);
    app.initialize();
    app.run();
    return 0;
}
