#include "includes/Packagemanager.hpp"


using namespace ara::ucm::pkgmgr;
using namespace std;


ara::ucm::TransferStartReturnType PackageManagerImpl::TransferStart(uint64_t Size)
{
    ara::ucm::TransferStartReturnType StartReturn =  ara::ucm::transfer::SoftwarePackage::TransferStart(Size);
    return StartReturn;
}

ara::ucm::OperationResultType  PackageManagerImpl::TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter)
{
    ara::ucm::OperationResultType DataReturn =  ara::ucm::transfer::SoftwarePackage::TransferData(id, data, blockCounter);
    return DataReturn;
}

ara::ucm::OperationResultType PackageManagerImpl::TransferExit(TransferIdType &id)
{
    ara::ucm::OperationResultType ExitReturn =  ara::ucm::transfer::SoftwarePackage::TransferExit(id);
    return ExitReturn;
}


void PackageManagerImpl::ProcessSwPackage(TransferIdType &id)
{
    string StringID { Convert2StringID(id) };

    /* GET PACKAGE */
    std::string SWPackagePath{ZIP_PackagesPath + "TransferID:" + StringID + ".zip"} ;

    /* USED TO PARSE */
    ara::ucm::parsing::SoftwarePackageParser SWParser_instance;

    SWParser_instance.UnzipFile(SWPackagePath);
    
    SWParser_instance.SwPackageManifestParser(SWPackagePath);

    SwClusterInfoType NewSwClusterInfo {SWParser_instance.GetSwClusterInfo(SWPackagePath)};

    ActionType action { SWParser_instance.GetActionType() };

    /* CHECK ACTION TYPE AND ACT ACCORDING TO IT */
    if ( action == ActionType::kUpdate )
    {
        // UpdateInstance should have a constructor that takes PackagePath 
        shared_ptr<storage::UpdateAction> updateInstance;
        ara::ucm::storage::SWCLManager::AddSWCLChangeInfo(NewSwClusterInfo , updateInstance);
        updateInstance->Execute();
    }

    else if ( action == ActionType::kInstall )
    {
        shared_ptr<storage::InstallAction> installInstance;
        ara::ucm::storage::SWCLManager::AddSWCLChangeInfo(NewSwClusterInfo , installInstance);
        installInstance->Execute();
    }
    
    else if ( action == ActionType::kRemove )
    {
        shared_ptr<storage::InstallAction> removeInstance;
        ara::ucm::storage::SWCLManager::AddSWCLChangeInfo(NewSwClusterInfo , removeInstance);
        removeInstance->Execute();
    }

    else
    {
        // WRONG ACTION TYPE
    }

    return;
}

