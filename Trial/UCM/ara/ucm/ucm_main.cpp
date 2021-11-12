#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "includes/Transfer.hpp"

using namespace std;
using namespace ara::ucm;

ara::ucm::ByteVectorType  ReadZipInBytes(const char * ZipPath);

int main (void)
{
    cout << "Eshta8lt!!!!" << endl;

    std::ifstream in;
    uint64_t length;
    uint64_t stop;;
    
    in.open("//home//tabakh//Desktop//GP//src//pkgs//test.zip");      // open input file
    in.seekg(0, std::ios::end);    // go to the end
    length = in.tellg();           // report location (this is the length)
    in.seekg(0, std::ios::beg);    // go back to the beginning
    char * StringData = new char[length];   // allocate memory for a buffer of appropriate dimension
    in.read(StringData, length);       // read the whole file into the buffer
    stop = in.gcount();
    in.close();     

    ara::ucm::ByteVectorType ByteData, TotalByteData;
    ara::ucm::transfer::SoftwarePackage SWPKG;

    //TotalByteData = ReadZipInBytes("//home//yasmin/Desktop//Graduation_Project//Test.txt");
    //length = TotalByteData.size();
    ara::ucm::TransferStartReturnType StartReturn = SWPKG.TransferStart(length);
    uint32_t BlockNumber = ceil((float) length / (float) StartReturn.BlockSize);
    uint32_t BlockCounter = 0;
    uint64_t BlockSize = StartReturn.BlockSize;
    uint8_t i = 0;
    
    while (BlockCounter < BlockNumber)
    {
        for(uint8_t j = 0; (j< BlockSize)&&((i+j)< length) ;j++)
        {
            ByteData.push_back(StringData[i+j]);
        }
        SWPKG.TransferData(StartReturn.id , ByteData, BlockCounter);
        BlockCounter++;
        ByteData.clear();
        i+=BlockSize;
    }

    SWPKG.TransferExit(StartReturn.id);    

    cout<<"5lst"<<endl;
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


