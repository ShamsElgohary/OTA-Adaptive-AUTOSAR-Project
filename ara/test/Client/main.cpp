#include "TestProxy.hpp"
#include <iostream>

using namespace ara::ucm::pkgmgr::proxy;
using namespace ara::com::proxy;
using namespace std ;


int main ()
{
    ara::com::ServiceHandleContainer<ProxyBase::HandleType> handles = PackageManagerProxy::FindService(1);
    PackageManagerProxy RecService(handles[0]);
    AddOutput r = RecService.Add(1,2);

    cout<< r.Sum;


    return 0;
}