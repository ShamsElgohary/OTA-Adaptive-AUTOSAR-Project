#include "includes/Packagemanager.hpp"


using namespace ara::ucm::pkgmgr;
using namespace std;

ara::log logger;

/* INITIALIZE CURRENT STATUS */
PackageManagerStatusType PackageManagerImpl::CurrentStatus = PackageManagerStatusType::kIdle;


ara::ucm::TransferStartReturnType PackageManagerImpl::TransferStart(uint64_t Size)
{
    ara::ucm::TransferStartReturnType StartReturn =  ara::ucm::transfer::SoftwarePackage::TransferStart(Size);
    
    logger.ActionsLog("[Transfer Start] \t\t Result : ", static_cast<unsigned char>(StartReturn.TransferStartResult) ); 

    return StartReturn;
}


ara::ucm::OperationResultType  PackageManagerImpl::TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter)
{
    ara::ucm::OperationResultType DataReturn =  ara::ucm::transfer::SoftwarePackage::TransferData(id, data, blockCounter);

    logger.ActionsLog("[Transfer Data] \t\t Result : ", static_cast<unsigned char>(DataReturn) );
    
    return DataReturn;
}


ara::ucm::OperationResultType PackageManagerImpl::TransferExit(TransferIdType &id)
{
    ara::ucm::OperationResultType ExitReturn =  ara::ucm::transfer::SoftwarePackage::TransferExit(id);

    logger.ActionsLog("[Transfer Exit] \t\t Result : ", static_cast<unsigned char>(ExitReturn) );

    return ExitReturn;
}


ara::ucm::OperationResultType PackageManagerImpl::TransferDelete(TransferIdType &id)
{
    ara::ucm::OperationResultType DeleteReturn =  ara::ucm::transfer::SoftwarePackage::TransferDelete(id);

    logger.ActionsLog("[Transfer Delete] \t\t Result : ", static_cast<unsigned char>(DeleteReturn) );

    return DeleteReturn;
}


ara::ucm::OperationResultType PackageManagerImpl::ProcessSwPackage(TransferIdType &id)
{
    ara::ucm::OperationResultType ProcessReturn;  
    
    try
    {
        ProcessReturn == PackageManagerStateinstance->ProcessSwPackage(id);
    }

    catch(...)
    {
        ProcessReturn = ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    logger.ActionsLog("[Process SwPackage] \t\t Result : ", static_cast<unsigned char>(ProcessReturn) );

    return ProcessReturn;
}


ara::ucm::OperationResultType PackageManagerImpl::Activate()
{
    ara::ucm::OperationResultType ActivateReturn = PackageManagerStateinstance->Activate();

    logger.ActionsLog("[Activate] \t\t Result : ", static_cast<unsigned char>(ActivateReturn) );

    return ActivateReturn;
}


vector <ara::ucm::SwClusterInfoType> PackageManagerImpl::GetSwClusterInfo()
{
   vector <ara::ucm::SwClusterInfoType> getSwCLuserReturn = ara::ucm::storage::SWCLManager::GetPresentSWCLs();
   
   logger.ActionsLog("[GetSwClusterInfo] \t\t Result : ", 0 );
   
   return getSwCLuserReturn; 
}


ara::ucm::OperationResultType PackageManagerImpl::Rollback()
{
    ara::ucm::OperationResultType RollbackResult = PackageManagerStateinstance->Rollback();

    logger.ActionsLog("[Rollback] \t\t Result : ", static_cast<unsigned char>(RollbackResult) );

    return RollbackResult;
}	


ara::ucm::OperationResultType PackageManagerImpl::RevertProcessedSwPackages()
{
    ara::ucm::OperationResultType RevertResult = PackageManagerStateinstance->Rollback();

    logger.ActionsLog("[RevertProcessedSwPackages] \t\t Result : ", static_cast<unsigned char>(RevertResult) );

    return RevertResult;
}	



PackageManagerStatusType PackageManagerImpl::GetCurrentStatus()
{
    logger.ActionsLog("[GetCurrentStatus] \t\t Result : ", 0 );

    return PackageManagerStateinstance->GetStatus();
}	




