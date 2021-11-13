#include "includes/Transfer.hpp"
/* Map is used in the .cpp */
#include "includes/SynchronizedStorage.hpp"

using json = nlohmann::json;
using namespace ara::ucm::transfer;
using namespace ara::ucm;


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
    string jsonPath = "PackageManagerPipe.json";

	/* Number Of Blocks to Read, Size in bytes and each 4 bytes are 1 block */
	StartTransferOutput.BlockSize = NewPackage.GetPackageBlockSize();

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
    std::string command;
    std::string ProjectPath = GetCurrentDirectory() ;
    std::string ZIP_PackagesPath = ProjectPath + "/ZIP_Packages/";
    /* Check first if path exists, if not create the folder ZIP_Packages*/
    if(!IsPathExist(ZIP_PackagesPath.c_str()))
    {
        command = "mkdir " + ZIP_PackagesPath;
        system(command.c_str());
    }
    

    string ParentPath = GetCurrentDirectory();
    /* change dir to path of packages folder */
    chdir(ZIP_PackagesPath.c_str());

    /* Create Zipped File */
    ofstream MyFile("TransferID:" + StringID + ".zip");
    //MyFile.write(&StringID[0],StringID.size());
    MyFile.close();


    /* Expected Size in Bytes */
    NewPackage.SetPackageExpectedBytes(Size);
    NewPackage.SetPackageExpectedBlocks(ceil(((float) Size)/NewPackage.GetPackageBlockSize()));

    /* Path of the created File to write data into */
    NewPackage.SetPackagePath(ZIP_PackagesPath); 
    /* Assign ID to the Package */
    NewPackage.SetPackageId(StartTransferOutput.id);
    NewPackage.SetPackageState( SwPackageStateType::kTransferring );

    /* ADD THE PACKAGE WITH ITS ID TO THE SYNCHRONIZED STORAGE 
    ( MAP IS STATIC SO MAP "Data" IS SHARED BETWEEN THE SS CLASS AND WON'T BE DELETED EXCEPT MANUALLY) */
    ara::ucm::SynchronizedStorage::AddItem(StartTransferOutput.id, NewPackage);
    
   /* FIRST MAKING THE JSON FILE MODIFIABLE */
    nlohmann::json UCM_PipeData;

    UCM_PipeData = jsonReadFromFile(jsonPath);

    /* WRITING OUR OPERATION INPUT */
    UCM_PipeData.at("UCM_OTA").at("TransferStart").at("Input").at("Size") = Size;
    /* WRITING OUR OPERATION OUTPUT */
    UCM_PipeData.at("UCM_OTA").at("TransferStart").at("Output").at("ID") =  StringID;

    /* WRITING THE ADJUSTMENTS TO THE FILE */
    jsonWriteInFile(jsonPath,UCM_PipeData);

    StartTransferOutput.TransferStartResult = TransferStartSuccessType::kSuccess;

    /* change dir to path of parent folder */
    chdir(ParentPath.c_str());

	return StartTransferOutput;
}



ara::ucm::OperationResultType  ara::ucm::transfer::SoftwarePackage::TransferData (ara::ucm::TransferIdType &id, ara::ucm::ByteVectorType data, uint64_t blockCounter)
{    
    /************** ERROR CHECKING **************/
    
    ara::ucm::OperationResultType ret = ara::ucm::OperationResultType::kSuccess;
	/* Check ID validity & Get SoftwarePackage  (CurrentSoftwarePackages > Vector of struct of SwPackageInfoType) */
	
	ara::ucm::transfer::SoftwarePackage * SwPkg = ara::ucm::SynchronizedStorage::GetItem(id);
	if (SwPkg == nullptr)
	{
		cout << "[UCM TRANSFER DATA] InvalidTransferId" << endl;
		ret = ara::ucm::OperationResultType::kInvalidTransferId;
		return ret;
	}

    /* Change The ID to String To get paths */
    ara::ucm::TransferIdType TransferID;
    SwPkg -> GetPackageId(TransferID);
    string StringID = to_string(TransferID[0]);   
    for (uint8_t i =1 ; i < 16 ; i++)
    {
      StringID += '-' + to_string(TransferID[i]);
    }
    string ParentPath = GetCurrentDirectory();
    string PackagePath = GetCurrentDirectory()+ "/ZIP_Packages/";
    /* change dir to path of packages folder */
    chdir(PackagePath.c_str());

	/* FIRST MAKING THE JSON FILE MODIFIABLE */
    nlohmann::json UCM_PipeData;
    UCM_PipeData = jsonReadFromFile("PackageManagerPipe.json");

	/* Check for size of received block */
	if (data.size() > (SwPkg -> GetPackageBlockSize()))
	{
		cout << "[UCM TRANSFER DATA] IncorrectBlockSize" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectBlockSize;
		/* Write Output to JSON File */
		UCM_PipeData.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
        /* WRITING THE ADJUSTMENTS TO THE FILE */
        jsonWriteInFile("PackageManagerPipe.json",UCM_PipeData);
		return ret;
	}

	/* Validate Size with Transfer Instance */
	uint64_t Size = SwPkg -> GetPackageExpectedBytes();
	if (((SwPkg->GetPackageReceivedBytes() + data.size()) > Size))
	{
		cout << "[UCM TRANSFER DATA] IncorrectSize" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectSize;
		/* Write Output to JSON File */
		UCM_PipeData.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
        /* WRITING THE ADJUSTMENTS TO THE FILE */
        jsonWriteInFile("PackageManagerPipe.json",UCM_PipeData);
		return ret;
	}

	/* Vaildate Block Counter */
	if (blockCounter != (SwPkg -> GetPackageReceivedBlocks()))
	{
		cout << "[UCM TRANSFER DATA] IncorrectBlock" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectBlock;
		/* Write Output to JSON File */
		UCM_PipeData.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
        /* WRITING THE ADJUSTMENTS TO THE FILE */
        jsonWriteInFile("PackageManagerPipe.json",UCM_PipeData);
		return ret;
	}

    /************** UPDATE ZIP FILE **************/

    /* Constract the Path */
    string zipName = "TransferID:" + StringID + ".zip";

    /* Open File To Update */
    std::ofstream zipIn(zipName, ios::app);

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


	//UCM_PipeData.at("UCM_OTA").at("TransferData").at("Input").at("dataBlock") = stringData;

	/* Increment ConsecutiveBytesReceived & ConsecutiveBlocksReceived */

	SwPkg -> SetPackageReceivedBytes(data.size() + (SwPkg -> GetPackageReceivedBytes()));
    SwPkg -> SetPackageReceivedBlocks(1 + (SwPkg -> GetPackageReceivedBlocks()));

	/* Write Output to JSON File */
	UCM_PipeData.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
	/* WRITING THE ADJUSTMENTS TO THE FILE */
    jsonWriteInFile("PackageManagerPipe.json",UCM_PipeData);  

    /* change dir to path of packages folder */
    chdir(ParentPath.c_str());

	return ret;
}

ara::ucm::OperationResultType ara::ucm::transfer::SoftwarePackage::TransferExit(ara::ucm::TransferIdType &id)
{
	ara::ucm::transfer::SoftwarePackage *SwPkg = ara::ucm::SynchronizedStorage::GetItem(id);

    ara::ucm::OperationResultType ret = ara::ucm::OperationResultType::kSuccess;

    /*************************ERROR CHECKING***************************/

    string ParentPath = GetCurrentDirectory();
    string PackagePath = GetCurrentDirectory()+ "/ZIP_Packages/";
    /* change dir to path of packages folder */
    chdir(PackagePath.c_str());

    /* FIRST MAKING THE JSON FILE MODIFIABLE */
    nlohmann::json UCM_PipeData;
    UCM_PipeData=jsonReadFromFile("PackageManagerPipe.json");
    
    /* Change The ID to String */
    ara::ucm::TransferIdType TransferID;
    SwPkg -> GetPackageId(TransferID);
    string StringID = to_string(TransferID[0]);   
    for (uint8_t i =1 ; i < 16 ; i++)
    {
      StringID += '-' + to_string(TransferID[i]);
    }

    //1- Authentication

    //2- Package Version

    //3- Size
    // gives an error if total transferred data size does not match expected data size provided with TransferStart
    if (SwPkg->GetPackageReceivedBytes() != SwPkg->GetPackageExpectedBytes())
    {
        cout << "[UCM TRANSFER EXIT] InsufficientData" << endl;
		ret = ara::ucm::OperationResultType::kInsufficientData;
		/* Write Output to JSON File */
        UCM_PipeData.at("UCM_OTA").at("TransferExit").at("Input").at("ID") = StringID;
		UCM_PipeData.at("UCM_OTA").at("TransferExit").at("Output").at("err") = ret;
        /* WRITING THE ADJUSTMENTS TO THE FILE */
        jsonWriteInFile("PackageManagerPipe.json",UCM_PipeData);
        /* change dir to path of packages folder */
        chdir(ParentPath.c_str());
		return ret;
    }

    //4- Manifest checking


    //5- ID
    //3ala hasab el implementation elly han3mlo l GetId()
    if (SwPkg == nullptr)
	{
		cout << "[UCM TRANSFER EXIT] InvalidTransferId" << endl;
		ret = ara::ucm::OperationResultType::kInvalidTransferId;
        /* Write Output to JSON File */
        UCM_PipeData.at("UCM_OTA").at("TransferExit").at("Input").at("ID") = StringID;
        UCM_PipeData.at("UCM_OTA").at("TransferExit").at("Output").at("err") = ret;
        /* WRITING THE ADJUSTMENTS TO THE FILE */
        jsonWriteInFile("PackageManagerPipe.json",UCM_PipeData);
        /* change dir to path of packages folder */
        chdir(ParentPath.c_str());
		return ret;
    }
    /* Write Output to JSON File */
    UCM_PipeData.at("UCM_OTA").at("TransferExit").at("Input").at("ID") = StringID;
    UCM_PipeData.at("UCM_OTA").at("TransferExit").at("Output").at("err") = ret;
    /* WRITING THE ADJUSTMENTS TO THE FILE */
    jsonWriteInFile("PackageManagerPipe.json",UCM_PipeData);

    /* change dir to path of packages folder */
    chdir(ParentPath.c_str());

    return ret;
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

void ara::ucm::transfer::SoftwarePackage::SetPackageExpectedBlocks(uint32_t expectedBlocks)
{
    TransferInfo.SetExpectedBlocks(expectedBlocks);
}

void ara::ucm::transfer::SoftwarePackage::SetPackageReceivedBlocks(uint32_t receivedBlocks)
{
    TransferInfo.SetReceivedBlocks(receivedBlocks);
}

void ara::ucm::transfer::SoftwarePackage::SetPackageBlockSize (uint32_t blockSize)
{
    TransferInfo.SetBlockSize(blockSize);
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

uint32_t ara::ucm::transfer::SoftwarePackage::GetPackageExpectedBlocks()
{
    return TransferInfo.GetExpectedBlocks();
}
uint32_t ara::ucm::transfer::SoftwarePackage::GetPackageReceivedBlocks()
{
    return TransferInfo.GetReceivedBlocks();
}

uint32_t ara::ucm::transfer::SoftwarePackage::GetPackageBlockSize ()
{
    return TransferInfo.GetBlockSize();
}

string ara::ucm::transfer::SoftwarePackage::GetPackagePath()
{
    return TransferInfo.GetTransferPath();
}

SwPackageStateType ara::ucm::transfer::SoftwarePackage::GetPackageState()
{
    return TransferInfo.GetTransferState();
}

void ara::ucm::transfer::SoftwarePackage::GetPackageId(ara::ucm::TransferIdType &TransferID)
{
    TransferInfo.GetTransferId(TransferID);
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

void ara::ucm::transfer::TransferInstance::SetExpectedBlocks(uint32_t expectedBlocks)
{
    this->expectedBlocks = expectedBlocks;
}

void ara::ucm::transfer::TransferInstance::SetReceivedBlocks(uint32_t receivedBlocks)
{
    this->receivedBlocks = receivedBlocks;
}

void ara::ucm::transfer::TransferInstance::SetBlockSize (uint32_t blockSize)
{
    this->blockSize = blockSize;
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

uint32_t ara::ucm::transfer::TransferInstance::GetExpectedBlocks()
{
    return expectedBlocks;
}

uint32_t ara::ucm::transfer::TransferInstance::GetReceivedBlocks()
{
    return receivedBlocks;
}

uint32_t ara::ucm::transfer::TransferInstance::GetBlockSize ()
{
    return blockSize;
}

string ara::ucm::transfer::TransferInstance::GetTransferPath()
{
    return path;
}

SwPackageStateType ara::ucm::transfer::TransferInstance::GetTransferState()
{
    return TransferState;
}

void ara::ucm::transfer::TransferInstance::GetTransferId(ara::ucm::TransferIdType &TransferID)
{
    for(uint8_t i = 0; i < 16; ++i)
    {
        TransferID[i] = transferId[i];
    }
}
