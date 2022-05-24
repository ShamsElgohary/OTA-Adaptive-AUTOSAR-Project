#include "../includes/Transfer.hpp"
#include "../includes/SynchronizedStorage.hpp"

using namespace ara::ucm;


namespace ara::ucm::transfer
{

    /*////////////////////////////////////////////////////////////////////////////

        SOFTWARE PACKAGE CLASS : TRANSFER METHODS USED TO TRANSFER PACKAGES

    ////////////////////////////////////////////////////////////////////////////*/

    ara::ucm::TransferStartReturnType SoftwarePackage::TransferStartInternal(uint64_t Size)
    {
        /* This will hold the data that will be returned from this operation */
        TransferStartReturnType StartTransferOutput;

        /* Initialize random seed: */
        srand(time(NULL));

        /* Generate a Transfer ID  */
        for (uint8_t i = 0; i < 16; i++)
        {
            StartTransferOutput.id[i] = rand() % UINT8_MAX;
        }

        /* If Path Doesn't Exist,Then Make Directory,Then Create Zipped File */
        if (!IsPathExist(ZIP_PackagesPath.c_str()))
        {
            command = "mkdir " + ZIP_PackagesPath;
            system(&command[0]);
        }

        string StringID{Convert2StringID(StartTransferOutput.id)};
        string FilePath{ZIP_PackagesPath + "/TransferID:" + StringID + ".zip"};

        /* CREATE A ZIPPED FILE */
        ofstream MyFile(FilePath);
        MyFile.close();

        /* Information of the NewPackage to be transfered */
        shared_ptr<SoftwarePackage> NewPackagePtr = make_shared<SoftwarePackage>(Size, FilePath, SwPackageStateType::kTransferring, StartTransferOutput.id);

        /* ADD THE PACKAGE WITH ITS ID TO THE SYNCHRONIZED STORAGE
        ( MAP IS STATIC SO MAP "Data" IS SHARED BETWEEN THE SS CLASS AND WON'T BE DELETED EXCEPT MANUALLY) */
        ara::ucm::SynchronizedStorage::AddItem(StartTransferOutput.id, NewPackagePtr);

        StartTransferOutput.BlockSize = NewPackagePtr->TransferInfo.GetBlockSize();
        StartTransferOutput.TransferStartResult = OperationResultType::kSuccess;

        // JSON (GUI SIMULATION)
        GUI_Logger.pkgAction("kTransferring",1);

        return StartTransferOutput;
    }

    ara::ucm::OperationResultType SoftwarePackage::TransferDataInternal(ara::ucm::TransferIdType id, ara::ucm::ByteVectorType data, uint64_t blockCounter)
    {
        /* Check ID validity & Get SoftwarePackage  (CurrentSoftwarePackages > Vector of struct of SwPackageInfoType) */
        shared_ptr<SoftwarePackage> SwPkg = ara::ucm::SynchronizedStorage::GetItem(id);
        if (SwPkg == nullptr)
        {
            return ara::ucm::OperationResultType::kInvalidTransferId;
        }

        /* Check for size of received block */
        if (data.size() > (SwPkg->GetPackageBlockSize()))
        {
            return ara::ucm::OperationResultType::kIncorrectBlockSize;
        }

        /* Validate Size with Transfer Instance */
        uint64_t Size = SwPkg->GetPackageExpectedBytes();
        if (SwPkg->GetPackageReceivedBytes() > Size)
        {
            return ara::ucm::OperationResultType::kIncorrectSize;
        }

        /* Vaildate Block Counter */
        if (blockCounter != (SwPkg->GetPackageReceivedBlocks()))
        {
            return ara::ucm::OperationResultType::kIncorrectBlock;
        }

        /************** UPDATE ZIP FILE **************/

        /* Construct the Path */
        string zipName = SwPkg->GetPackagePath();

        /* Open File To Update */
        std::ofstream zipIn(zipName, ios::binary | ios::app);

        /* Append Data to zip File */
        for (uint8_t byte : data)
        {
            zipIn << byte;
        }

        /* Close File */
        zipIn.close();

        /* Increment ConsecutiveBytesReceived & ConsecutiveBlocksReceived */
        SwPkg->SetPackageReceivedBytes(data.size() + (SwPkg->GetPackageReceivedBytes()));
        SwPkg->SetPackageReceivedBlocks(1 + (SwPkg->GetPackageReceivedBlocks()));

        SwPkg->UpdateFinishRate();
        
        // GUI SIMULATION
        GUI_Logger.ReportJsonGUI( "TransferFinishRate" , SwPkg->GetFinishRate()*100);
        // DELAY TO SHOW RATE?

        return ara::ucm::OperationResultType::kSuccess;
    }

    ara::ucm::OperationResultType SoftwarePackage::TransferExitInternal(ara::ucm::TransferIdType id)
    {
        /* Check ID validity & Get SoftwarePackage To Get Its Path */
        shared_ptr<SoftwarePackage> SwPkg = ara::ucm::SynchronizedStorage::GetItem(id);
        if (SwPkg == nullptr)
        {
            return ara::ucm::OperationResultType::kInvalidTransferId;
        }

        // 1- Authentication

        // 2- Package Version

        // Gives an error if total transferred data size does not match expected data size provided with TransferStart
        if (SwPkg->GetPackageReceivedBytes() != SwPkg->GetPackageExpectedBytes())
        {
            return ara::ucm::OperationResultType::kInsufficientData;
        }

        // 4- Manifest checking

        SwPkg->SetPackageState(SwPackageStateType::kTransferred);

        // JSON (GUI SIMULATION)
        GUI_Logger.pkgAction("kTransferred",1);

        return ara::ucm::OperationResultType::kSuccess;
    }

    ara::ucm::OperationResultType SoftwarePackage::TransferDeleteInternal(TransferIdType id)
    {
        /* Check ID validity & Get SoftwarePackage To Get Its Path */
        shared_ptr<SoftwarePackage> SwPkg = ara::ucm::SynchronizedStorage::GetItem(id);
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
    SoftwarePackage::SoftwarePackage(uint64_t expectedBytes, string path, ara::ucm::SwPackageStateType TransferState, TransferIdType transferId)
    {
        this->TransferInfo.SetExpectedBytes(expectedBytes);
        this->TransferInfo.SetTransferPath(path);
        this->TransferInfo.SetTransferState(TransferState);
        this->TransferInfo.SetTransferId(transferId);
        this->TransferInfo.SetExpectedBlocks(ceil(((float)expectedBytes) / this->TransferInfo.GetBlockSize()));
    }

    /* DEFAULT CONSTRUCTOR */
    SoftwarePackage::SoftwarePackage()
    {
    }

    /*////////////////////////////////////////////////////////////////////////////

        METHODS RELATED TO THE TransferInfo OBJECT SINCE DATA IS ENCAPSULATED

    ////////////////////////////////////////////////////////////////////////////*/

    void SoftwarePackage::SetPackageExpectedBytes(uint64_t expectedBytes)
    {
        TransferInfo.SetExpectedBytes(expectedBytes);
    }

    void SoftwarePackage::SetPackageReceivedBytes(uint64_t receivedBytes)
    {
        TransferInfo.SetReceivedBytes(receivedBytes);
    }

    void SoftwarePackage::SetPackageExpectedBlocks(uint32_t expectedBlocks)
    {
        TransferInfo.SetExpectedBlocks(expectedBlocks);
    }

    void SoftwarePackage::SetPackageReceivedBlocks(uint32_t receivedBlocks)
    {
        TransferInfo.SetReceivedBlocks(receivedBlocks);
    }

    void SoftwarePackage::SetPackageBlockSize(uint32_t blockSize)
    {
        TransferInfo.SetBlockSize(blockSize);
    }

    void SoftwarePackage::SetPackagePath(string path)
    {
        TransferInfo.SetTransferPath(path);
    }

    void SoftwarePackage::SetPackageState(SwPackageStateType State)
    {
        TransferInfo.SetTransferState(State);
    }

    void SoftwarePackage::SetPackageId(TransferIdType Id)
    {
        TransferInfo.SetTransferId(Id);
    }

    void SoftwarePackage::UpdateFinishRate()
    {
        TransferInfo.UpdateFinishRate();
    }

    uint64_t SoftwarePackage::GetPackageExpectedBytes()
    {
        return TransferInfo.GetExpectedBytes();
    }

    uint64_t SoftwarePackage::GetPackageReceivedBytes()
    {
        return TransferInfo.GetReceivedBytes();
    }

    uint32_t SoftwarePackage::GetPackageExpectedBlocks()
    {
        return TransferInfo.GetExpectedBlocks();
    }
    uint32_t SoftwarePackage::GetPackageReceivedBlocks()
    {
        return TransferInfo.GetReceivedBlocks();
    }

    uint32_t SoftwarePackage::GetPackageBlockSize()
    {
        return TransferInfo.GetBlockSize();
    }

    string SoftwarePackage::GetPackagePath()
    {
        return TransferInfo.GetTransferPath();
    }

    SwPackageStateType SoftwarePackage::GetPackageState()
    {
        return TransferInfo.GetTransferState();
    }

    TransferIdType SoftwarePackage::GetPackageId()
    {
        return TransferInfo.GetTransferId();
    }

    float SoftwarePackage::GetFinishRate()
    {
        return TransferInfo.GetFinishRate();
    }

    /*////////////////////////////////////////////////////////////////////////////

                         CLASS TRANSFER INSTANCE METHODS

    ////////////////////////////////////////////////////////////////////////////*/

    void TransferInstance::SetExpectedBytes(uint64_t expectedBytes)
    {
        /* SET EXPECTED BYTES OF THIS OBJECT TO THE GIVEN EXPECTED BYTES */
        this->expectedBytes = expectedBytes;
    }

    void TransferInstance::SetTransferPath(string path)
    {
        this->path = path;
    }

    void TransferInstance::SetReceivedBytes(uint64_t receivedBytes)
    {
        this->receivedBytes = receivedBytes;
    }

    void TransferInstance::SetExpectedBlocks(uint32_t expectedBlocks)
    {
        this->expectedBlocks = expectedBlocks;
    }

    void TransferInstance::SetReceivedBlocks(uint32_t receivedBlocks)
    {
        this->receivedBlocks = receivedBlocks;
    }

    void TransferInstance::SetBlockSize(uint32_t blockSize)
    {
        this->blockSize = blockSize;
    }

    void TransferInstance::SetTransferState(SwPackageStateType TransferState)
    {
        this->TransferState = TransferState;
    }

    void TransferInstance::SetTransferId(TransferIdType transferId)
    {
        this->transferId = transferId;
    }

    void TransferInstance::UpdateFinishRate()
    {
        TransferFinishRate = (float)receivedBytes / expectedBytes;
    }

    uint64_t TransferInstance::GetExpectedBytes()
    {
        return expectedBytes;
    }

    uint64_t TransferInstance::GetReceivedBytes()
    {
        return receivedBytes;
    }

    uint32_t TransferInstance::GetExpectedBlocks()
    {
        return expectedBlocks;
    }

    uint32_t TransferInstance::GetReceivedBlocks()
    {
        return receivedBlocks;
    }

    uint32_t TransferInstance::GetBlockSize()
    {
        return blockSize;
    }

    string TransferInstance::GetTransferPath()
    {
        return path;
    }

    SwPackageStateType TransferInstance::GetTransferState()
    {
        return TransferState;
    }

    TransferIdType TransferInstance::GetTransferId()
    {
        return transferId;
    }

    float TransferInstance::GetFinishRate()
    {
        return TransferFinishRate;
    }


} // End of namespace ara::ucm::transfer