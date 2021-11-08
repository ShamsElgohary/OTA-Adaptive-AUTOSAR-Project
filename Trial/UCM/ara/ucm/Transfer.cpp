#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include "nlohmann/json.hpp"

#include "UCM_Types.hpp"
#include "Transfer.hpp"


using json = nlohmann::json;
using namespace ara::ucm::transfer;
using namespace ara::ucm;



ara::ucm::TransferStartReturnType ara::ucm::transfer::SoftwarePackage::TransferStart(uint64_t Size)
{
    /* This  is the file where ota will read from */
    string jsonPath = "UCM_PackageManagerPipe.json";

	/* Information of the NewPackage to be transfered */
	SoftwarePackage NewPackageInfo;

	/* This will hold the data that will be returned from this operation */
	TransferStartReturnType StartTransferOutput;

	/* Number Of Blocks, Size in bytes and each 4 bytes are 1 block */
	StartTransferOutput.BlockSize = ceil(Size / 4);

    /* Initialize random seed: */
	srand (time(NULL));

	/* Generate a Transfer ID  */
    for (uint8_t i =0 ; i < 16 ; i++)
    {
    	  NewPackageInfo.transferId[i] = rand() % UINT8_MAX ;
          StartTransferOutput.id[i] = NewPackageInfo.transferId[i];
    }

	/* So OTA Can Read ID */
	string StringID = to_string(NewPackageInfo.transferId[0]);   
    for (uint8_t i =1 ; i < 16 ; i++)
    {
      StringID += '-' + to_string(NewPackageInfo.transferId[i] );
    }

    /* Initiliaze receive bytes to zero (None yet) */
    NewPackageInfo.receivedBytes = 0;

    /* Get path of packages folder */
    NewPackageInfo.path = GetCurrentDirectory()+ "//Packages";
    /* change dir to path of packages folder */
    chdir(NewPackageInfo.path.c_str());

    /* Create Zipped File */
    ofstream MyFile("TransferID:" + StringID + ".zip");
    //MyFile.write(&StringID[0],StringID.size());
    MyFile.close();

    ara::ucm::SynchronizedStorage::AddItem(NewPackageInfo.transferId, NewPackageInfo);

    /* FIRST MAKING THE JSON FILE MODIFIABLE */
    nlohmann::json UCM_PipeData;

    ifstream JSON_Modifiable(jsonPath);
    JSON_Modifiable >> UCM_PipeData;

    /* WRITING OUR OPERATION INPUT */
    UCM_PipeData.at("UCM_OTA").at("TransferStart").at("Input").at("Size") = Size;
    /* WRITING OUR OPERATION OUTPUT */
    UCM_PipeData.at("UCM_OTA").at("TransferStart").at("Output").at("ID") =  StringID;

    /* WRITING THE ADJUSTMENTS TO THE FILE */
    ofstream JSON_PipeFile( NewPackageInfo.path );
	JSON_PipeFile<<setw(4)<< UCM_PipeData <<endl;

    StartTransferOutput.TransferStartResult = TransferStartSuccessType::kSuccess;
    NewPackageInfo.TransferState = SwPackageStateType::kTransferring;
    
	return StartTransferOutput;
}

ara::ucm::OperationResultType  ara::ucm::transfer::SoftwarePackage::TransferData (ara::ucm::TransferIdType id, ara::ucm::ByteVectorType data, uint64_t blockCounter)
{

}

ara::ucm::OperationResultType ara::ucm::transfer::SoftwarePackage::TransferExit(ara::ucm::TransferIdType id)
{

}


/* Used to get the path of our current directory */
string GetCurrentDirectory()
{
    char directoryPath[FILENAME_MAX];
    getcwd(directoryPath,FILENAME_MAX);
    return directoryPath;
}