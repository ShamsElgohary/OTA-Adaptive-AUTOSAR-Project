#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "includes/PackageManager.hpp"

using namespace std;
using namespace ara::ucm;

int main (void)
{
    ara::ucm::pkgmgr::PackageManagerImpl PackageManagerInstance;
    string path =  ZIP_PackagesPath + "/PackageTest.zip";

    ifstream ifs(path, ios::binary | ios::ate);
    ifstream::pos_type pos = ifs.tellg();
    vector<char> result(pos);
    ifs.seekg(0, ios::beg);
    ifs.read(&result[0], pos);

    ara::ucm::ByteVectorType ByteData;

    ara::ucm::TransferStartReturnType StartReturn = PackageManagerInstance.TransferStart(result.size());
    uint32_t BlockNumber = ceil((float) result.size() / (float) StartReturn.BlockSize);
    uint32_t BlockCounter = 0;
    uint64_t BlockSize = StartReturn.BlockSize;
    int i = 0;

    while (BlockCounter < BlockNumber)
    {
    for(uint32_t j = 0; (j< BlockSize)&&((i+j)< result.size()) ;j++)
        {
            ByteData.push_back(result[i+j]);
        }

        PackageManagerInstance.TransferData(StartReturn.id , ByteData, BlockCounter);
        BlockCounter++;
        ByteData.clear();
        i+=BlockSize;
    }
    
    PackageManagerInstance.TransferExit(StartReturn.id);

    PackageManagerInstance.ProcessSwPackage(StartReturn.id);

    PackageManagerInstance.Activate();

    //PackageManagerInstance.Rollback();

    PackageManagerInstance.Finish();

    return 0;
}




