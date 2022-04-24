#include "PackageManagerProxy.hpp"
#include <iostream>

using namespace ara::ucm::pkgmgr::proxy;
using namespace ara::com::proxy;
using namespace std;

string Convert2StringID(TransferIdType TransferID);

int main()
{
    ara::com::ServiceHandleContainer<ProxyBase::HandleType> handles = PackageManagerProxy::FindService(1);
    PackageManagerProxy RecService(handles[0]);
    TransferStartOutput r = RecService.TransferStart(1700);

    cout << "Result :" << endl;
    cout << "Block Size: " << r.BlockSize << endl;
    cout << "Transfer id: " << Convert2StringID(r.id) << endl;
    return 0;
}


string Convert2StringID(TransferIdType TransferID)
{
    string StringID = "";   
    for (auto i : TransferID)
    {
      StringID += '-' + to_string(i);
    }
    return StringID;
}