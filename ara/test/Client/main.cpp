#include "PackageManagerProxy.hpp"
#include <iostream>
#include "execution_client.hpp"

using namespace ara::ucm::pkgmgr::proxy;
using namespace ara::com::proxy;
using namespace std;

string Convert2StringID(TransferIdType TransferID);
string Convert2String(ByteVectorType Data);

int main()
{
    ara::exec::ExecutionClient exec;
    exec.ReportExecutionStaste(ara::exec::ExecutionState::Krunning);

    
    ara::com::ServiceHandleContainer<ProxyBase::HandleType> handles = PackageManagerProxy::FindService(1);

    if (handles.size())
    {
        PackageManagerProxy RecService(handles[0]);
        // cout<<"find service returned 1 or more handle"<<endl;
        // cout<<RecService.Add(3,4).Sum<<endl;
        ifstream ifs(CUSTOMIZED_PROJECT_PATH + "executables/com_proxy_test/0.1/bin/PackageTest#3.zip", ios::binary | ios::ate);
        ifstream::pos_type pos = ifs.tellg();
        vector<char> result(pos);
        ifs.seekg(0, ios::beg);
        ifs.read(&result[0], pos);

        ByteVectorType ByteData;

        TransferStartOutput Startrtn = RecService.TransferStart(result.size());

        uint32_t BlockNumber = ceil((float)result.size() / (float)Startrtn.BlockSize);
        uint32_t BlockCounter = 0;
        uint64_t BlockSize = Startrtn.BlockSize;
        int i = 0;

        while (BlockCounter < BlockNumber)
        {
            for (uint32_t j = 0; (j < BlockSize) && ((i + j) < result.size()); j++)
            {
                ByteData.push_back(result[i + j]);
            }
            RecService.TransferData(Startrtn.id, ByteData, BlockCounter);

            BlockCounter++;
            ByteData.clear();
            i += BlockSize;
        }

        RecService.TransferExit(Startrtn.id);


        RecService.ProcessSwPackage(Startrtn.id);
        RecService.Activate();
        // RecService.Rollback();
        RecService.Finish(); 
    }
    
    return 0;
}

string Convert2StringID(TransferIdType TransferID)
{
    string StringID = "";
    for (auto i : TransferID)
        StringID += '-' + to_string(i);
    return StringID;
}

string Convert2String(ByteVectorType Data)
{
    string String = "";
    for (auto i : Data)
        String += '-' + to_string(i);
    return String;
}