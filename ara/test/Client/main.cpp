// #include "PackageManagerProxy.hpp"
#include "TestProxy.hpp"
#include <iostream>

using namespace ara::ucm::pkgmgr::proxy;
using namespace ara::com::proxy;
using namespace std;

int main()
{
    ara::com::ServiceHandleContainer<ProxyBase::HandleType> handles = PackageManagerProxy::FindService(1);
    if (handles.size())
    {
        PackageManagerProxy RecService(handles[0]);
        AddOutput r = RecService.Add(3, 4);
        cout << r.Sum << endl;
    }
    return 0;
}
