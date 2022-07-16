#include "../includes/PackageManager.hpp"
#include "../../log/log.hpp"

using namespace std;


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

        GUI_Logger.ClearActionsLog();
        GUI_Logger.ActionsLog("[TransferStart] \t\t Result : ", static_cast<unsigned char>(StartReturn.TransferStartResult));
        GUI_Logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::TransferDataOutput>
    PackageManagerImpl::TransferData(TransferIdType id, ByteVectorType data, uint64_t blockCounter)
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::TransferDataOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::TransferDataOutput DataReturn{ara::ucm::transfer::SoftwarePackage::TransferDataInternal(id, data, blockCounter)};

        promise.set_value(DataReturn);

        GUI_Logger.ActionsLog("[TransferData] \t\t Result : ", static_cast<unsigned char>(DataReturn.OperationReturn));

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::TransferExitOutput> PackageManagerImpl::TransferExit(TransferIdType id)
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::TransferExitOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::TransferExitOutput ExitReturn{ara::ucm::transfer::SoftwarePackage::TransferExitInternal(id)};

        promise.set_value(ExitReturn);

        GUI_Logger.ActionsLog("[TransferExit] \t\t Result : ", static_cast<unsigned char>(ExitReturn.OperationReturn));

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::TransferDeleteOutput> PackageManagerImpl::TransferDelete(TransferIdType id)
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::TransferDeleteOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::TransferDeleteOutput DeleteReturn{ara::ucm::transfer::SoftwarePackage::TransferDeleteInternal(id)};

        promise.set_value(DeleteReturn);

        GUI_Logger.ActionsLog("[TransferDelete] \t\t Result : ", static_cast<unsigned char>(DeleteReturn.OperationReturn));

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

        catch (const std::exception & ex)
        {
            ProcessReturn.OperationReturn = ara::ucm::OperationResultType::kOperationNotPermitted;
        }


        promise.set_value(ProcessReturn);

        GUI_Logger.ActionsLog("[ProcessSwPackage] \t\t Result : ", static_cast<unsigned char>(ProcessReturn.OperationReturn));
        GUI_Logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::ActivateOutput> PackageManagerImpl::Activate()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::ActivateOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::ActivateOutput ActivateReturn{PackageManagerStateinstance->ActivateInternal()};

        promise.set_value(ActivateReturn);

        GUI_Logger.ActionsLog("[Activate] \t\t Result : ", static_cast<unsigned char>(ActivateReturn.OperationReturn));
        GUI_Logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::GetSwClusterInfoOutput> PackageManagerImpl::GetSwClusterInfo()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::GetSwClusterInfoOutput> promise;

        ara::ucm::SwClusterInfoVectorType PresetSWCulsters = ara::ucm::storage::SWCLManager::GetPresentSWCLs();

        ara::ucm::pkgmgr::PackageManagerImpl::GetSwClusterInfoOutput getSwCLuserReturn;

        getSwCLuserReturn.vectorOfClusterInfo = PresetSWCulsters;

        promise.set_value(getSwCLuserReturn);

        GUI_Logger.ActionsLog("[GetSwClusterInfo] \t\t Result : ", 0);
        GUI_Logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::RollbackOutput> PackageManagerImpl::Rollback()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::RollbackOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::RollbackOutput RollbackResult{PackageManagerStateinstance->RollbackInternal()};

        promise.set_value(RollbackResult);

        GUI_Logger.ActionsLog("[Rollback] \t\t Result : ", static_cast<unsigned char>(RollbackResult.OperationReturn));
        GUI_Logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::RevertProcessedSwPackagesOutput> PackageManagerImpl::RevertProcessedSwPackages()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::RevertProcessedSwPackagesOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::RevertProcessedSwPackagesOutput RevertResult{PackageManagerStateinstance->RollbackInternal()};

        promise.set_value(RevertResult);

        GUI_Logger.ActionsLog("[RevertProcessedSwPackages] \t\t Result : ", static_cast<unsigned char>(RevertResult.OperationReturn));
        GUI_Logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::FinishOutput> PackageManagerImpl::Finish()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::FinishOutput> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::FinishOutput FinishResult{PackageManagerStateinstance->FinishInternal()};

        promise.set_value(FinishResult);

        GUI_Logger.ActionsLog("[Finish] \t\t Result : ", static_cast<unsigned char>(FinishResult.OperationReturn));
        // GUI_Logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

    std::future<ara::ucm::pkgmgr::PackageManagerImpl::CurrentStatusField> PackageManagerImpl::GetCurrentStatus()
    {
        std::promise<ara::ucm::pkgmgr::PackageManagerImpl::CurrentStatusField> promise;

        ara::ucm::pkgmgr::PackageManagerImpl::CurrentStatusField StatusResult;
        StatusResult.CurrentStatus = PackageManagerStateinstance->GetStatusInternal();

        promise.set_value(StatusResult);

        GUI_Logger.ActionsLog("[GetCurrentStatus] \t\t Result : ", 0);
        GUI_Logger.ReportStatus( static_cast<unsigned char>( PackageManagerStateinstance->GetStatusInternal() ) );

        return promise.get_future();
    }

} // end of namespace ara::ucm::pkgmgr
