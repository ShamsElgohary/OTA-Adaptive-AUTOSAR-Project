#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "includes/PackageManager.hpp"

using namespace std;
using namespace ara::ucm;

ara::ucm::ByteVectorType  ReadZipInBytes(const char * ZipPath);

int main (void)
{
    ara::ucm::pkgmgr::PackageManagerImpl PackageManagerInstance;
    string path =  ZIP_PackagesPath + "/PackageTest.zip";

    ifstream ifs(path, ios::binary | ios::ate);
    ifstream::pos_type pos = ifs.tellg();
    vector<char> result(pos);
    ifs.seekg(0, ios::beg);
    ifs.read(&result[0], pos);

    ara::ucm::ByteVectorType ByteData, TotalByteData;

    ara::ucm::TransferStartReturnType StartReturn = PackageManagerInstance.TransferStart(result.size());
    uint32_t BlockNumber = ceil((float) result.size() / (float) StartReturn.BlockSize);
    uint32_t BlockCounter = 0;
    uint64_t BlockSize = StartReturn.BlockSize;
    int i = 0;

    while (BlockCounter < BlockNumber)
    {

   
    for(uint8_t j = 0; (j< BlockSize)&&((i+j)< result.size()) ;j++)
        {
            ByteData.push_back(result[i+j]);
        }

        PackageManagerInstance.TransferData(StartReturn.id , ByteData, BlockCounter);
        BlockCounter++;
        ByteData.clear();
        i+=BlockSize;
    }

    cout <<"FINALLY WORKING \n";

    PackageManagerInstance.TransferExit(StartReturn.id);

    PackageManagerInstance.ProcessSwPackage(StartReturn.id);
}

ara::ucm::ByteVectorType  ReadZipInBytes(const char * ZipPath)
{
    ifstream ifs(ZipPath, ios::binary | ios::ate);
    ifstream::pos_type pos = ifs.tellg();
    vector<char> result(pos);
    ifs.seekg(0, ios::beg);
    ifs.read(&result[0], pos);
    ara::ucm::ByteVectorType resultByte;
    for (uint8_t i=0; i<result.size(); i++)
    {
        resultByte.push_back(result[i]);
    }
    return resultByte;
}



