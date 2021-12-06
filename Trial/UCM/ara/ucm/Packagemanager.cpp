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
    ara::ucm::OperationResultType ProcessReturn =  PackageManagerStateinstance->ProcessSwPackage(id);

    logger.ActionsLog("[Process SwPackage] \t\t Result : ", static_cast<unsigned char>(ProcessReturn) );

    return ProcessReturn;
}
