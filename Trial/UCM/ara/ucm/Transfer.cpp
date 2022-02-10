#include "includes/Transfer.hpp"
#include "includes/SynchronizedStorage.hpp"

using namespace ara::ucm::transfer;
using namespace ara::ucm;


/*////////////////////////////////////////////////////////////////////////////

    SOFTWARE PACKAGE CLASS : TRANSFER METHODS USED TO TRANSFER PACKAGES

////////////////////////////////////////////////////////////////////////////*/


ara::ucm::TransferStartReturnType ara::ucm::transfer::SoftwarePackage::TransferStart(uint64_t Size)
{
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
    string FilePath {ZIP_PackagesPath + "/TransferID:" + StringID + ".zip" }; 

    /* CREATE A ZIPPED FILE */
    ofstream MyFile( FilePath );
    MyFile.close();

	/* Information of the NewPackage to be transfered */
    shared_ptr<SoftwarePackage> NewPackagePtr = make_shared<SoftwarePackage>(Size, FilePath, SwPackageStateType::kTransferring, StartTransferOutput.id );    

    /* ADD THE PACKAGE WITH ITS ID TO THE SYNCHRONIZED STORAGE 
    ( MAP IS STATIC SO MAP "Data" IS SHARED BETWEEN THE SS CLASS AND WON'T BE DELETED EXCEPT MANUALLY) */
    ara::ucm::SynchronizedStorage::AddItem(StartTransferOutput.id, NewPackagePtr );

	StartTransferOutput.BlockSize = NewPackagePtr->TransferInfo.GetBlockSize();
    StartTransferOutput.TransferStartResult = OperationResultType::kSuccess;
	return StartTransferOutput;
}




ara::ucm::OperationResultType  ara::ucm::transfer::SoftwarePackage::TransferData (ara::ucm::TransferIdType &id, ara::ucm::ByteVectorType data, uint64_t blockCounter)
{    
	/* Check ID validity & Get SoftwarePackage  (CurrentSoftwarePackages > Vector of struct of SwPackageInfoType) */
	shared_ptr<ara::ucm::transfer::SoftwarePackage> SwPkg = ara::ucm::SynchronizedStorage::GetItem(id);
	if (SwPkg == nullptr)
	{
		return ara::ucm::OperationResultType::kInvalidTransferId;
	}

	/* Check for size of received block */
	if (data.size() > (SwPkg -> GetPackageBlockSize()))
	{
		return ara::ucm::OperationResultType::kIncorrectBlockSize;
	}

	/* Validate Size with Transfer Instance */
	uint64_t Size = SwPkg -> GetPackageExpectedBytes();
	if ( SwPkg->GetPackageReceivedBytes() > Size )
	{
		return ara::ucm::OperationResultType::kIncorrectSize;
	}

	/* Vaildate Block Counter */
	if (blockCounter != (SwPkg -> GetPackageReceivedBlocks()))
	{
		return ara::ucm::OperationResultType::kIncorrectBlock;
	}

    /************** UPDATE ZIP FILE **************/

    /* Constract the Path */
    string zipName = SwPkg->GetPackagePath();

    /* Open File To Update */
    std::ofstream zipIn(zipName, ios::binary | ios::app);

    /* Append Data to zip File */
    for( uint8_t byte : data)
    {   
        zipIn << byte;
    }
    
    /* Close File */
    zipIn.close();

	/* Increment ConsecutiveBytesReceived & ConsecutiveBlocksReceived */
	SwPkg -> SetPackageReceivedBytes(data.size() + (SwPkg -> GetPackageReceivedBytes()));
    SwPkg -> SetPackageReceivedBlocks(1 + (SwPkg -> GetPackageReceivedBlocks()));

	return ara::ucm::OperationResultType::kSuccess;;
}



ara::ucm::OperationResultType ara::ucm::transfer::SoftwarePackage::TransferExit(ara::ucm::TransferIdType &id)
{
	/* Check ID validity & Get SoftwarePackage To Get Its Path */
	shared_ptr<ara::ucm::transfer::SoftwarePackage> SwPkg = ara::ucm::SynchronizedStorage::GetItem(id);
    if (SwPkg == nullptr)
	{
        return ara::ucm::OperationResultType::kInvalidTransferId;
    }

    //1- Authentication

    //2- Package Version

    // Gives an error if total transferred data size does not match expected data size provided with TransferStart
    if (SwPkg->GetPackageReceivedBytes() != SwPkg->GetPackageExpectedBytes())
    {
		return ara::ucm::OperationResultType::kInsufficientData;
    }

    //4- Manifest checking

    SwPkg ->SetPackageState(SwPackageStateType::kTransferred);

    return ara::ucm::OperationResultType::kSuccess;
}



ara::ucm::OperationResultType ara::ucm::transfer::SoftwarePackage::TransferDelete(TransferIdType &id)
{
	/* Check ID validity & Get SoftwarePackage To Get Its Path */
    shared_ptr<ara::ucm::transfer::SoftwarePackage> SwPkg = ara::ucm::SynchronizedStorage::GetItem(id);
    if (SwPkg == nullptr)
	{
        return ara::ucm::OperationResultType::kInvalidTransferId;
    }

    command = "rm " + SwPkg->GetPackagePath();
    system(&command[0]);
    
    SynchronizedStorage::DeleteItem(id);

    return ara::ucm::OperationResultType::kSuccess;
}


/*////////////////////////////////////////////////////////////////////////////

    CONSTRUCTOR OF SOFTWARE PACKAGE

////////////////////////////////////////////////////////////////////////////*/



/* CONSTRUCTOR FOR TRANSFER INSTANCE */
ara::ucm::transfer::SoftwarePackage::SoftwarePackage (uint64_t expectedBytes, string path, ara::ucm::SwPackageStateType TransferState, TransferIdType transferId)
{
    this->TransferInfo.SetExpectedBytes(expectedBytes);
    this->TransferInfo.SetTransferPath(path);
    this->TransferInfo.SetTransferState(TransferState);
    this->TransferInfo.SetTransferId(transferId);
    this->TransferInfo.SetExpectedBlocks( ceil(((float) expectedBytes) / this->TransferInfo.GetBlockSize()) );
}

/* DEFAULT CONSTRUCTOR */
ara::ucm::transfer::SoftwarePackage::SoftwarePackage()
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
