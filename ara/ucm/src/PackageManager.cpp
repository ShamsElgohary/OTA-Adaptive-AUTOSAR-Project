#include "../includes/PackageManager.hpp"
#include "../../log/log.hpp"

using namespace std;

ara::log logger;

namespace ara::ucm::pkgmgr
{

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::TransferStartOutput> PackageManagerImpl::TransferStart(uint64_t Size)
    {
        std::promise<TransferStartOutput> promise;

        ara::ucm::TransferStartReturnType initialReturn = ara::ucm::transfer::SoftwarePackage::TransferStartInternal(Size);

        TransferStartOutput StartReturn;
        StartReturn.BlockSize = initialReturn.BlockSize;
        StartReturn.TransferStartResult = initialReturn.TransferStartResult;
        StartReturn.id = initialReturn.id; // COPY 16 ELEMENTS OF ID FROM initialReturn TO StartReturn

        promise.set_value(StartReturn);

        logger.ActionsLog("[TransferStart] \t\t Result : ", static_cast<unsigned char>(StartReturn.TransferStartResult));
        logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::TransferDataOutput>
    PackageManagerImpl::TransferData(TransferIdType id, ByteVectorType data, uint64_t blockCounter)
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::TransferDataOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::TransferDataOutput DataReturn{ara::ucm::transfer::SoftwarePackage::TransferDataInternal(id, data, blockCounter)};

        promise.set_value(DataReturn);

        logger.ActionsLog("[TransferData] \t\t Result : ", static_cast<unsigned char>(DataReturn.OperationReturn));

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::TransferExitOutput> PackageManagerImpl::TransferExit(TransferIdType id)
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::TransferExitOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::TransferExitOutput ExitReturn{ara::ucm::transfer::SoftwarePackage::TransferExitInternal(id)};

        promise.set_value(ExitReturn);

        logger.ActionsLog("[TransferExit] \t\t Result : ", static_cast<unsigned char>(ExitReturn.OperationReturn));

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::TransferDeleteOutput> PackageManagerImpl::TransferDelete(TransferIdType id)
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::TransferDeleteOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::TransferDeleteOutput DeleteReturn{ara::ucm::transfer::SoftwarePackage::TransferDeleteInternal(id)};

        promise.set_value(DeleteReturn);

        logger.ActionsLog("[TransferDelete] \t\t Result : ", static_cast<unsigned char>(DeleteReturn.OperationReturn));

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::ProcessSwPackageOutput> PackageManagerImpl::ProcessSwPackage(TransferIdType id)
    {

        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::ProcessSwPackageOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::ProcessSwPackageOutput ProcessReturn;

        try
        {
            ProcessReturn.OperationReturn = PackageManagerStateinstance->ProcessSwPackageInternal(id);
        }

        catch (...)
        {
            ProcessReturn.OperationReturn = ara::ucm::OperationResultType::kOperationNotPermitted;
        }

        promise.set_value(ProcessReturn);

        logger.ActionsLog("[ProcessSwPackage] \t\t Result : ", static_cast<unsigned char>(ProcessReturn.OperationReturn));
        logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::ActivateOutput> PackageManagerImpl::Activate()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::ActivateOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::ActivateOutput ActivateReturn{PackageManagerStateinstance->ActivateInternal()};

        promise.set_value(ActivateReturn);

        logger.ActionsLog("[Activate] \t\t Result : ", static_cast<unsigned char>(ActivateReturn.OperationReturn));
        logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::GetSwClusterInfoOutput> PackageManagerImpl::GetSwClusterInfo()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::GetSwClusterInfoOutput> promise;

        ara::ucm::SwClusterInfoVectorType PresetSWCulsters = ara::ucm::storage::SWCLManager::GetPresentSWCLs();

        ara::ucm::pkgmgr::PackageManagerImpl::GetSwClusterInfoOutput getSwCLuserReturn;

        getSwCLuserReturn.vectorOfClusterInfo = PresetSWCulsters;

        promise.set_value(getSwCLuserReturn);

        logger.ActionsLog("[GetSwClusterInfo] \t\t Result : ", 0);
        logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::RollbackOutput> PackageManagerImpl::Rollback()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::RollbackOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::RollbackOutput RollbackResult{PackageManagerStateinstance->RollbackInternal()};

        promise.set_value(RollbackResult);

        logger.ActionsLog("[Rollback] \t\t Result : ", static_cast<unsigned char>(RollbackResult.OperationReturn));
        logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::RevertProcessedSwPackagesOutput> PackageManagerImpl::RevertProcessedSwPackages()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::RevertProcessedSwPackagesOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::RevertProcessedSwPackagesOutput RevertResult{PackageManagerStateinstance->RollbackInternal()};

        promise.set_value(RevertResult);

        logger.ActionsLog("[RevertProcessedSwPackages] \t\t Result : ", static_cast<unsigned char>(RevertResult.OperationReturn));
        logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::FinishOutput> PackageManagerImpl::Finish()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::FinishOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::FinishOutput FinishResult{PackageManagerStateinstance->FinishInternal()};

        promise.set_value(FinishResult);

        logger.ActionsLog("[Finish] \t\t Result : ", static_cast<unsigned char>(FinishResult.OperationReturn));
        logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::GetCurrentStatusField> PackageManagerImpl::GetCurrentStatus()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::GetCurrentStatusField> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::GetCurrentStatusField StatusResult;
        StatusResult.Status = PackageManagerStateinstance->GetStatusInternal();

        promise.set_value(StatusResult);

        logger.ActionsLog("[GetCurrentStatus] \t\t Result : ", 0);
        logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

} // end of namespace ara::ucm::pkgmgr
