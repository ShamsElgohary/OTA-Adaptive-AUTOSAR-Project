#include "TestProxy.hpp"
#include <iostream>

using namespace ara::ucm::pkgmgr::proxy;
using namespace ara::com::proxy;

int main ()
{
    ara::com::ServiceHandleContainer<ProxyBase::HandleType> handles = PackageManagerProxy::FindService(1);
    cout << "1...." << endl;
    cout << handles.size() << endl;
    PackageManagerProxy RecService(handles[0]);

    AddOutput r = RecService.Add(1,2);

    std::cout<< r.Sum;


    return 0;
}