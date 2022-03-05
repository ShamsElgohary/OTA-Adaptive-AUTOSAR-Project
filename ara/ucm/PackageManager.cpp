#include "includes/PackageManager.hpp"
#include "../log/log.hpp"


using namespace std;

ara::log logger;

namespace ara::ucm::pkgmgr
{

/* INITIALIZE CURRENT STATUS */
PackageManagerStatusType PackageManagerImpl::CurrentStatus = PackageManagerStatusType::kIdle;


std::future<ara::com::skeleton::PackageManagerSkeleton::TransferStartOutput> PackageManagerImpl::TransferStart(uint64_t Size)
{
    std::promise<TransferStartOutput> promise;

    ara::ucm::TransferStartReturnType initialReturn = ara::ucm::transfer::SoftwarePackage::TransferStart(Size);

    TransferStartOutput StartReturn;
    StartReturn.BlockSize = initialReturn.BlockSize;
    StartReturn.TransferStartResult = initialReturn.TransferStartResult;
    std::copy(initialReturn.id, initialReturn.id+16, StartReturn.id); //COPY 16 ELEMENTS OF ID FROM initialReturn TO StartReturn

    promise.set_value(StartReturn);
    
    logger.ActionsLog("[Transfer Start] \t\t Result : ", static_cast<unsigned char>(StartReturn.TransferStartResult) ); 

    return promise.get_future();
}



std::future<ara::com::skeleton::PackageManagerSkeleton::OperationResultOutput> 
    PackageManagerImpl::TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter)
{
    std::promise<OperationResultOutput> promise;

    OperationResultOutput DataReturn{ ara::ucm::transfer::SoftwarePackage::TransferData(id, data, blockCounter) };

    promise.set_value(DataReturn);
    
    logger.ActionsLog("[Transfer Data] \t\t Result : ", static_cast<unsigned char>(DataReturn.OperationReturn) );
    
    return promise.get_future();
}



std::future<ara::com::skeleton::PackageManagerSkeleton::OperationResultOutput>  PackageManagerImpl::TransferExit(TransferIdType &id)
{
    std::promise<OperationResultOutput> promise;

    OperationResultOutput ExitReturn{ ara::ucm::transfer::SoftwarePackage::TransferExit(id) };

    promise.set_value(ExitReturn);

    logger.ActionsLog("[Transfer Exit] \t\t Result : ", static_cast<unsigned char>(ExitReturn.OperationReturn) );

    return promise.get_future();
}


std::future<ara::com::skeleton::PackageManagerSkeleton::OperationResultOutput> PackageManagerImpl::TransferDelete(TransferIdType &id)
{
    std::promise<OperationResultOutput> promise;

    OperationResultOutput DeleteReturn{ ara::ucm::transfer::SoftwarePackage::TransferDelete(id) };

    promise.set_value(DeleteReturn);

    logger.ActionsLog("[Transfer Delete] \t\t Result : ", static_cast<unsigned char>(DeleteReturn.OperationReturn) );

    return promise.get_future();
}


std::future<ara::com::skeleton::PackageManagerSkeleton::OperationResultOutput> PackageManagerImpl::ProcessSwPackage(TransferIdType &id)
{
    
    std::promise<OperationResultOutput> promise;

    OperationResultOutput ProcessReturn;  
    
    try
    {
        ProcessReturn.OperationReturn = PackageManagerStateinstance->ProcessSwPackage(id);
    }

    catch(...)
    {
        ProcessReturn.OperationReturn = ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    promise.set_value(ProcessReturn);

    logger.ActionsLog("[Process SwPackage] \t\t Result : ", static_cast<unsigned char>(ProcessReturn.OperationReturn) );

    return promise.get_future();
}


std::future<ara::com::skeleton::PackageManagerSkeleton::OperationResultOutput> PackageManagerImpl::Activate()
{
    std::promise<OperationResultOutput> promise;

    OperationResultOutput ActivateReturn{ PackageManagerStateinstance->Activate() };

    promise.set_value(ActivateReturn);

    logger.ActionsLog("[Activate] \t\t Result : ", static_cast<unsigned char>(ActivateReturn.OperationReturn) );

    return promise.get_future();
}


std::future<ara::com::skeleton::PackageManagerSkeleton::GetSWClusterInfoOutput> PackageManagerImpl::GetSwClusterInfo()
{
   std::promise<GetSWClusterInfoOutput> promise;
    
   GetSWClusterInfoOutput getSwCLuserReturn{ ara::ucm::storage::SWCLManager::GetPresentSWCLs() };

   promise.set_value(getSwCLuserReturn);
   
   logger.ActionsLog("[GetSwClusterInfo] \t\t Result : ", 0 );
   
   return promise.get_future(); 
}


std::future<ara::com::skeleton::PackageManagerSkeleton::OperationResultOutput> PackageManagerImpl::Rollback()
{
    std::promise<OperationResultOutput> promise;
    
    OperationResultOutput RollbackResult{ PackageManagerStateinstance->Rollback() };

    promise.set_value(RollbackResult);

    logger.ActionsLog("[Rollback] \t\t Result : ", static_cast<unsigned char>(RollbackResult.OperationReturn) );

    return promise.get_future();
}	


std::future<ara::com::skeleton::PackageManagerSkeleton::OperationResultOutput> PackageManagerImpl::RevertProcessedSwPackages()
{
    std::promise<OperationResultOutput> promise;
    
    OperationResultOutput RevertResult{ PackageManagerStateinstance->Rollback() };

    promise.set_value(RevertResult);

    logger.ActionsLog("[RevertProcessedSwPackages] \t\t Result : ", static_cast<unsigned char>(RevertResult.OperationReturn) );

    return promise.get_future();
}	



std::future<ara::com::skeleton::PackageManagerSkeleton::GetCurrentStatusOutput> PackageManagerImpl::GetCurrentStatus()
{
    std::promise<GetCurrentStatusOutput> promise;

    GetCurrentStatusOutput StatusResult{ PackageManagerStateinstance->GetStatus() };

    promise.set_value(StatusResult);

    logger.ActionsLog("[GetCurrentStatus] \t\t Result : ", 0 );

    return promise.get_future();
}	


std::future<ara::com::skeleton::PackageManagerSkeleton::OperationResultOutput> PackageManagerImpl::Finish()
{
    std::promise<OperationResultOutput> promise;
    
    OperationResultOutput FinishResult{ PackageManagerStateinstance->Finish() };

    promise.set_value(FinishResult);

    logger.ActionsLog("[Activate] \t\t Result : ", static_cast<unsigned char>(FinishResult.OperationReturn) );

    return promise.get_future();
}


PackageManagerImpl::PackageManagerImpl()
{
    // DEFAULT CONSTRUCTOR
}

PackageManagerImpl::~PackageManagerImpl()
{
    // DEFAULT DESTRUCTOR
}   


}// end of namespace ara::ucm::pkgmgr
