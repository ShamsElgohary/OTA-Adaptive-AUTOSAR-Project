#include "TestProxy.hpp"
#include <iostream>

using namespace ara::ucm::pkgmgr::proxy;
using namespace ara::com::proxy;

int main ()
{
    ara::com::ServiceHandleContainer<ProxyBase::HandleType> handles = PackageManagerProxy::FindService();
    PackageManagerProxy RecService(handles[1]);
    AddOutput r = RecService.Add(1,2);
    std::cout<< r.Sum;

    return 0;
}