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
	/* Information of the NewPackage to be transfered */
	SoftwarePackage NewPackage;

	/* This will hold the data that will be returned from this operation */
	TransferStartReturnType StartTransferOutput;

    /* This  is the file where ota will read from */
    string jsonPath = "UCM_PackageManagerPipe.json";
    string PackagePath;

	/* Number Of Blocks to Read, Size in bytes and each 4 bytes are 1 block */
	StartTransferOutput.BlockSize = ceil(Size / 4);

    /* Initialize random seed: */
	srand (time(NULL));

	/* Generate a Transfer ID  */
    for (uint8_t i =0 ; i < 16 ; i++){
    	StartTransferOutput.id[i] = rand() % UINT8_MAX ;
    }

	/* So OTA Can Read ID */
	string StringID = to_string(StartTransferOutput.id[0]);   
    for (uint8_t i =1 ; i < 16 ; i++)
    {
      StringID += '-' + to_string(StartTransferOutput.id[i] );
    }

    /* Get path of packages folder */
    PackagePath = GetCurrentDirectory()+ "//Packages";
    /* change dir to path of packages folder */
    chdir(PackagePath.c_str());

    /* Create Zipped File */
    ofstream MyFile("TransferID:" + StringID + ".zip");
    //MyFile.write(&StringID[0],StringID.size());
    MyFile.close();


    /* Expected Size in Bytes */
    NewPackage.SetPackageExpectedBytes(Size);
    /* Path of the created File to write data into */
    NewPackage.SetPackagePath(PackagePath); 
    /* Assign ID to the Package */
    NewPackage.SetPackageId(StartTransferOutput.id);
    NewPackage.SetPackageState( SwPackageStateType::kTransferring );

    /* ADD THE PACKAGE WITH ITS ID TO THE SYNCHRONIZED STORAGE 
    ( MAP IS STATIC SO MAP "Data" IS SHARED BETWEEN THE SS CLASS AND WON'T BE DELETED EXCEPT MANUALLY) */
    ara::ucm::SynchronizedStorage::AddItem(StartTransferOutput.id, NewPackage);
    
   /* FIRST MAKING THE JSON FILE MODIFIABLE */
    nlohmann::json UCM_PipeData;

    ifstream JSON_Modifiable(jsonPath);
    JSON_Modifiable >> UCM_PipeData;

    /* WRITING OUR OPERATION INPUT */
    UCM_PipeData.at("UCM_OTA").at("TransferStart").at("Input").at("Size") = Size;
    /* WRITING OUR OPERATION OUTPUT */
    UCM_PipeData.at("UCM_OTA").at("TransferStart").at("Output").at("ID") =  StringID;

    /* WRITING THE ADJUSTMENTS TO THE FILE */
    ofstream JSON_PipeFile( jsonPath );
	JSON_PipeFile<<setw(4)<< UCM_PipeData <<endl;


    StartTransferOutput.TransferStartResult = TransferStartSuccessType::kSuccess;
	return StartTransferOutput;
}



ara::ucm::OperationResultType  ara::ucm::transfer::SoftwarePackage::TransferData (ara::ucm::TransferIdType id, ara::ucm::ByteVectorType data, uint64_t blockCounter)
{

}

ara::ucm::OperationResultType ara::ucm::transfer::SoftwarePackage::TransferExit(ara::ucm::TransferIdType id)
{

}



/*////////////////////////////////////////////////////////////////////////////

    METHODS RELATED TO THE TransferInfo OBJECT SINCE DATA IS ENCAPSULATED

////////////////////////////////////////////////////////////////////////////*/


void ara::ucm::transfer::SoftwarePackage::SetPackageExpectedBytes(uint64_t expectedBytes)
{
    TransferInfo.SetExpectedBytes(expectedBytes);
}

void ara::ucm::transfer::SoftwarePackage::SetPackageReceivedBytes(uint64_t receivedBytes)
{
    TransferInfo.SetReceivedBytes(receivedBytes);
}

void ara::ucm::transfer::SoftwarePackage::SetPackagePath(string path)
{
    TransferInfo.SetTransferPath(path);
}

void ara::ucm::transfer::SoftwarePackage::SetPackageState(SwPackageStateType State)
{
    TransferInfo.SetTransferState(State);
}

void ara::ucm::transfer::SoftwarePackage::SetPackageId(TransferIdType Id)
{
    TransferInfo.SetTransferId(Id);
}



/*////////////////////////////////////////////////////////////////////////////

                     CLASS TRANSFER INSTANCE METHODS

////////////////////////////////////////////////////////////////////////////*/


void ara::ucm::transfer::TransferInstance::SetExpectedBytes(uint64_t expectedBytes)
{
    /* SET EXPECTED BYTES OF THIS OBJECT TO THE GIVEN EXPECTED BYTES */
    this->expectedBytes = expectedBytes;
}

void ara::ucm::transfer::TransferInstance::SetTransferPath(string path)
{  
    this->path = path;
}

void ara::ucm::transfer::TransferInstance::SetReceivedBytes(uint64_t receivedBytes)
{
    this->receivedBytes = receivedBytes;
}

void ara::ucm::transfer::TransferInstance::SetTransferState(SwPackageStateType TransferState)
{
    this->TransferState = TransferState;
}

void ara::ucm::transfer::TransferInstance::SetTransferId(TransferIdType transferId)
{
    /* COPY GIVEN ID */
    for( uint8_t i = 0 ; i < 16 ; i++)
    {
        this->transferId[i] = transferId[i];
    }
}

uint64_t ara::ucm::transfer::TransferInstance::GetExpectedBytes()
{
    return expectedBytes;
}

uint64_t ara::ucm::transfer::TransferInstance::GetReceivedBytes()
{
    return receivedBytes;
}

string ara::ucm::transfer::TransferInstance::GetTransferPath()
{
    return path;
}

SwPackageStateType ara::ucm::transfer::TransferInstance::GetTransferState()
{
    return TransferState;
}

TransferIdType &ara::ucm::transfer::TransferInstance::GetTransferId()
{
    return transferId;
}






/*////////////////////////////////////////////////////////////////////////////

                HELPER FUNCTIONS USED IN THE ABOVE METHODS

////////////////////////////////////////////////////////////////////////////*/


/* Used to get the path of our current directory */
string GetCurrentDirectory()
{
    char directoryPath[FILENAME_MAX];
    getcwd(directoryPath,FILENAME_MAX);
    return directoryPath;
}