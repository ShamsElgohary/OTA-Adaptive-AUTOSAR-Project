#include <iostream>
#include <fstream>
#include <cmath>
#include "includes/UCM_Types.hpp"
#include "includes/Transfer.hpp"

using namespace std;
using namespace ara::ucm::transfer;

int main (void)
{
    cout << "Eshta8lt!!!!" << endl;
    ifstream in ("//home//yasmin/Desktop//Graduation_Project//Test.txt");
    string StringData; 
    in >> StringData;
    ara::ucm::ByteVectorType ByteData;

    for(uint64_t i = 0; i<StringData.length(); i++)
    {
        ByteData.push_back(StringData[i]);
    }

    ara::ucm::transfer::SoftwarePackage SWPKG;

    ara::ucm::TransferStartReturnType StartReturn = SWPKG.TransferStart(214);
    uint32_t BlockNumber = ceil((float) 214 / (float) StartReturn.BlockSize);
    uint32_t BlockCounter = 0;
    while (BlockCounter > BlockNumber)
    {
        SWPKG.TransferData(StartReturn.id , ByteData, BlockCounter);
        BlockCounter++;
    }
}


