#include "TestProxy.hpp"
#include <iostream>

using namespace ara::ucm::pkgmgr::proxy;

int main ()
{
    ara::com::ServiceHandleContainer<PackageManagerProxy::HandleType> handles = PackageManagerProxy::FindService();
    PackageManagerProxy RecService(handles[1]);
    AddOutput r = RecService.Add(1,2);
    std::cout<< r.Sum;

    return 0;
}