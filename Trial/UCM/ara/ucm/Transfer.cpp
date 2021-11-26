#include "includes/Transfer.hpp"
#include "includes/SynchronizedStorage.hpp"

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

    /* Initialize random seed: */
	srand (time(NULL));

	/* Generate a Transfer ID  */
    for (uint8_t i =0 ; i < 16 ; i++){
    	StartTransferOutput.id[i] = rand() % UINT8_MAX ;
    }

    /* If Path Doesn't Exist,Then Make Directory,Then Create Zipped File */
    if(!IsPathExist(ZIP_PackagesPath.c_str()))
    {
        command = "mkdir " + ZIP_PackagesPath;
        system(&command[0]);
    }

    string StringID { Convert2StringID(StartTransferOutput.id) };
    ofstream MyFile( ZIP_PackagesPath + "/TransferID:" + StringID + ".zip");
    MyFile.close();

    /* SET ALL DATA OF THE 'NewPackage' THAT IS TO BE TRANSFERED (Size in Bytes , Number of Blocks, etc.) */
    NewPackage.SetPackageExpectedBytes(Size);
    NewPackage.SetPackageExpectedBlocks(ceil(((float) Size)/NewPackage.GetPackageBlockSize()));
    NewPackage.SetPackagePath(ZIP_PackagesPath); 
    NewPackage.SetPackageId(StartTransferOutput.id);
    NewPackage.SetPackageState( SwPackageStateType::kTransferring );

    /* ADD THE PACKAGE WITH ITS ID TO THE SYNCHRONIZED STORAGE 
    ( MAP IS STATIC SO MAP "Data" IS SHARED BETWEEN THE SS CLASS AND WON'T BE DELETED EXCEPT MANUALLY) */
    ara::ucm::SynchronizedStorage::AddItem(StartTransferOutput.id, NewPackage);

	StartTransferOutput.BlockSize = NewPackage.GetPackageBlockSize();
    StartTransferOutput.TransferStartResult = TransferStartSuccessType::kSuccess;
	return StartTransferOutput;
}


ara::ucm::OperationResultType  ara::ucm::transfer::SoftwarePackage::TransferData (ara::ucm::TransferIdType &id, ara::ucm::ByteVectorType data, uint64_t blockCounter)
{    
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
    string StringID { Convert2StringID(TransferID) };

	/* Check for size of received block */
	if (data.size() > (SwPkg -> GetPackageBlockSize()))
	{
		cout << "[UCM TRANSFER DATA] IncorrectBlockSize" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectBlockSize;
		return ret;
	}

	/* Validate Size with Transfer Instance */
	uint64_t Size = SwPkg -> GetPackageExpectedBytes();
	if ( SwPkg->GetPackageReceivedBytes() > Size )
	{
		cout << "[UCM TRANSFER DATA] IncorrectSize" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectSize;
		return ret;
	}

	/* Vaildate Block Counter */
	if (blockCounter != (SwPkg -> GetPackageReceivedBlocks()))
	{
		cout << "[UCM TRANSFER DATA] IncorrectBlock" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectBlock;
		return ret;
	}

    /************** UPDATE ZIP FILE **************/

    /* Constract the Path */
    string zipName = ZIP_PackagesPath + "/TransferID:" + StringID + ".zip";    

    /* Open File To Update */
    std::ofstream zipIn(zipName, ios::app);

    /* Convert Data to String */
    std::string stringData (reinterpret_cast<const char *>(&data[0]), data.size());

    static int i = 0;
    /* Append Data to zip File */
    zipIn << data[i];

    i++;
    
    /* Close File */
    zipIn.close();

	
	// std::string stringData (reinterpret_cast <char*>(data), sizeof(data));

	/* Increment ConsecutiveBytesReceived & ConsecutiveBlocksReceived */

	SwPkg -> SetPackageReceivedBytes(data.size() + (SwPkg -> GetPackageReceivedBytes()));
    SwPkg -> SetPackageReceivedBlocks(1 + (SwPkg -> GetPackageReceivedBlocks()));

	return ret;
}

ara::ucm::OperationResultType ara::ucm::transfer::SoftwarePackage::TransferExit(ara::ucm::TransferIdType &id)
{
	ara::ucm::transfer::SoftwarePackage *SwPkg = ara::ucm::SynchronizedStorage::GetItem(id);

    ara::ucm::OperationResultType ret = ara::ucm::OperationResultType::kSuccess;

    /*************************ERROR CHECKING***************************/


    
    /* Change The ID to String */
    ara::ucm::TransferIdType TransferID;
    SwPkg -> GetPackageId(TransferID);
    string StringID { Convert2StringID(TransferID) };

    //1- Authentication

    //2- Package Version

    //3- Size
    // gives an error if total transferred data size does not match expected data size provided with TransferStart

    if (SwPkg->GetPackageReceivedBytes() != SwPkg->GetPackageExpectedBytes())
    {
        cout << "[UCM TRANSFER EXIT] InsufficientData" << endl;
		ret = ara::ucm::OperationResultType::kInsufficientData;
		return ret;
    }

    //4- Manifest checking

    //5- ID
    
    //3ala hasab el implementation elly han3mlo l GetId()
    if (SwPkg == nullptr)
	{
		cout << "[UCM TRANSFER EXIT] InvalidTransferId" << endl;
		ret = ara::ucm::OperationResultType::kInvalidTransferId;
        /* change dir to path of packages folder */
		return ret;
    }

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
