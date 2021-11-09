#include "includes/Transfer.hpp"
/* Map is used in the .cpp */
#include "includes/SynchronizedStorage.hpp"

using json = nlohmann::json;
using namespace ara::ucm::transfer;
using namespace ara::ucm;


/*////////////////////////////////////////////////////////////////////////////

                FUNCTIONS USED IN THE BELOW METHODS

////////////////////////////////////////////////////////////////////////////*/


/* Used to get the path of our current directory */
string GetCurrentDirectory()
{
    char directoryPath[FILENAME_MAX];
    getcwd(directoryPath,FILENAME_MAX);
    return directoryPath;
}


/*////////////////////////////////////////////////////////////////////////////

    SOFTWARE PACKAGE CLASS : TRANSFER METHODS USED TO TRANSFER PACKAGES

////////////////////////////////////////////////////////////////////////////*/


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
	JSON_PipeFile<< setw(4)<< UCM_PipeData <<endl;

    StartTransferOutput.TransferStartResult = TransferStartSuccessType::kSuccess;
	return StartTransferOutput;
}



ara::ucm::OperationResultType  ara::ucm::transfer::SoftwarePackage::TransferData (ara::ucm::TransferIdType id, ara::ucm::ByteVectorType data, uint64_t blockCounter)
{    
    ara::ucm::transfer::SoftwarePackage *SwPkg;
    
    /************** ERROR CHECKING **************/
    
    ara::ucm::OperationResultType ret = ara::ucm::OperationResultType::kSuccess;
	/* Check ID validity & Get SoftwarePackage  (CurrentSoftwarePackages > Vector of struct of SwPackageInfoType) */
	
	// ara::ucm::transfer::SoftwarePackage * SwPkg = ara::ucm::transfer::SynchronizedStorage::GetItem(id);
	if (SwPkg == nullptr)
	{
		cout << "[UCM TRANSFER DATA] InvalidTransferId" << endl;
		ret = ara::ucm::OperationResultType::kInvalidTransferId;
		return ret;
	}

    /* Change The ID to String To get paths */
    string StringID = to_string(SwPkg -> transferId[0]);   
    for (uint8_t i =1 ; i < 16 ; i++)
    {
      StringID += '-' + to_string(SwPkg -> transferId[i]);
    }

	/* FIRST MAKING THE JSON FILE MODIFIABLE */
    nlohmann::json UCM_PipeData;
    ifstream JSON_Modifiable("PackageManagerPipe.json");
    JSON_Modifiable >> UCM_PipeData;

	/* Check for size of received block */
	if (data.size() > (SwPkg -> BlockSize))
	{
		cout << "[UCM TRANSFER DATA] IncorrectBlockSize" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectBlockSize;
		/* Write Output to JSON File */
		UCM_PipeData.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
        /* WRITING THE ADJUSTMENTS TO THE FILE */
		ofstream JSON_PipeFile("PackageManagerPipe.json");
		JSON_PipeFile<<setw(4)<< UCM_PipeData <<endl;
		return ret;
	}

	/* Validate Size with Transfer Instance */
	uint64_t Size = SwPkg->expectedBytes;
	if (((SwPkg->receivedBytes + data.size()) > Size))
	{
		cout << "[UCM TRANSFER DATA] IncorrectSize" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectSize;
		/* Write Output to JSON File */
		UCM_PipeData.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
        /* WRITING THE ADJUSTMENTS TO THE FILE */
		ofstream JSON_PipeFile("PackageManagerPipe.json");
		JSON_PipeFile<<setw(4)<< UCM_PipeData <<endl;
		return ret;
	}

	/* Vaildate Block Counter */
	if (blockCounter != (SwPkg -> receivedBlocks))
	{
		cout << "[UCM TRANSFER DATA] IncorrectBlock" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectBlock;
		/* Write Output to JSON File */
		UCM_PipeData.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
        /* WRITING THE ADJUSTMENTS TO THE FILE */
		ofstream JSON_PipeFile("PackageManagerPipe.json");
		JSON_PipeFile<<setw(4)<< UCM_PipeData <<endl;
		return ret;
	}

    /************** UPDATE ZIP FILE **************/
    /* Constract the Path */
    string zipPath = "TransferID:" + StringID + ".zip";

    /* Open File To Update */
    std::ofstream zipIn(zipPath, ios::app);

    /* Convert Data to String */
    std::string stringData (reinterpret_cast<const char *>(&data[0]), data.size());

    /* Append Data to zip File */
    zipIn << stringData;

    /* Close File */
    zipIn.close();

    /************** WRITE INPUT & OUTPUT DATA TO JSON FILE **************/
	/* Write Input to JSON File (ID -> First Time) & (BlockCounter -> Increment) & (Data -> Append) */
	if (blockCounter == 0)
	{
		UCM_PipeData.at("UCM_OTA").at("TransferData").at("Input").at("ID") = StringID;
	}
	UCM_PipeData.at("UCM_OTA").at("TransferData").at("Input").at("blockCounter") = blockCounter;
	
	// std::string stringData (reinterpret_cast <char*>(data), sizeof(data));
	UCM_PipeData.at("UCM_OTA").at("TransferData").at("Input").at("dataBlock") = stringData;

	/* Increment ConsecutiveBytesReceived & ConsecutiveBlocksReceived */
	SwPkg -> receivedBytes += data.size();
	SwPkg -> receivedBlocks += 1;

	/* Write Output to JSON File */
	UCM_PipeData.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
	/* WRITING THE ADJUSTMENTS TO THE FILE */
	ofstream JSON_PipeFile("PackageManagerPipe.json");
	JSON_PipeFile<<setw(4)<< UCM_PipeData <<endl;
	return ret;
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

uint64_t ara::ucm::transfer::SoftwarePackage::GetPackageExpectedBytes()
{
    return TransferInfo.GetExpectedBytes();
}
uint64_t ara::ucm::transfer::SoftwarePackage::GetPackageReceivedBytes()
{
   return TransferInfo.GetReceivedBytes();
}
string ara::ucm::transfer::SoftwarePackage::GetPackagePath()
{
    return TransferInfo.GetTransferPath();
}
SwPackageStateType ara::ucm::transfer::SoftwarePackage::GetPackageState()
{
    return TransferInfo.GetTransferState();
}
TransferIdType &ara::ucm::transfer::SoftwarePackage::GetPackageId()
{
    return TransferInfo.GetTransferId();
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
