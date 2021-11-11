#include <iostream>
#include <fstream>
#include <cmath>
#include "includes/Transfer.hpp"

using namespace std;
using namespace ara::ucm::transfer;

int main (void)
{
    cout << "Eshta8lt!!!!" << endl;

    std::ifstream in;
    int length;
    in.open("//home//yasmin/Desktop//Graduation_Project//Test.txt");      // open input file
    in.seekg(0, std::ios::end);    // go to the end
    length = in.tellg();           // report location (this is the length)
    in.seekg(0, std::ios::beg);    // go back to the beginning
    char * StringData = new char[length];   // allocate memory for a buffer of appropriate dimension
    in.read(StringData, length);       // read the whole file into the buffer
    in.close();     

    ara::ucm::ByteVectorType ByteData;
    ara::ucm::transfer::SoftwarePackage SWPKG;

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
}


