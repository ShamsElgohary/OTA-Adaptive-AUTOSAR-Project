#include "includes/State.hpp"
#include "includes/Storage.hpp"
using namespace ara::ucm::storage;
using namespace ara::ucm::state;

PackageManagerState::PackageManagerState(PackageManagerStatusType &pkgmgr_CurrentStatus)
{
    /* CURRENT STATUS IS REFERENCED TO THE STATUS IN THE INTERFACE PACKAGEMANAGER */
    this->CurrentStatus = &pkgmgr_CurrentStatus;
}

/* DEFAULT CONSTRUCTOR */
PackageManagerState::PackageManagerState()
{

}


void PackageManagerState::dependencyCheck(void)
{

}


/*we must write processes in process list*/

ara::ucm::OperationResultType PackageManagerState::Activate()
{

    if ((*CurrentStatus) != PackageManagerStatusType::kReady)
    {
        /* PKG not in ready state */
        /*Operation Not Permitted*/
        return ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    (*CurrentStatus) = PackageManagerStatusType::kActivating;
    /* Get all Clusters with kPresent State */
    vector<ara::ucm::SwClusterInfoType> PresentSWCls = SWCLManager::GetPresentSWCLs();

    /* Get all Clusters with kAdded/kRemoved/kUpdated State */
    vector<ara::ucm::SwClusterInfoType> ChangedSWCls = SWCLManager::GetSWCLsChangeInfo();

    /*Create ProcessList*/
    json processList;

    /*Depenency Missing Error Check*/
    dependencyCheck();

    /*if(SM::StartUpdateSession(functional_Group) == SM_ReceivedStates::kRejected)
        {
            UpdateSessionRejected Error
        }

    */

    /*

    uint8 RejectedCounter = 0;

    do
    {
        SM_ReceivedStates PrepareState = SM::PrepareUpdate(functional_Group);



        if(PrepareState == SM_ReceivedStates::kPrepareFailed)
        {
            (*CurrentStatus) = PackageManagerStatusType::kReady;

            PreActivation Failed Error

            return
            
        }
        else if (PrepareState == SM_ReceivedStates::kRejected)
        {
            if(RejectedCounter == 6 (hash define for rejected counter))
            {
                (*CurrentStatus) = PackageManagerStatusType::kReady;

                PreActivation Failed Error

                return
            }
            
            RejectedCounter++;
        }
    }while(PrepareState != kPrepared);

    RejectedCounter = 0;
    */

    /*

    do
    {
        SM_ReceivedStates VerifyState = SM::VerifyUpdate(functional_Group);
        (*CurrentStatus) = PackageManagerStatusType::kVerifing;


        if(VerifyState == SM_ReceivedStates::kVerifyFailed)
        {
            (*CurrentStatus) = PackageManagerStatusType::kReady;

            VerificationFailed Failed Error

            return
            
        }
        else if (VerifyState == SM_ReceivedStates::kRejected)
        {
            if(RejectedCounter == 6 (hash define for rejected counter))
            {
                Rollback();

                VerificationFailed Failed Error

                return
            }
            RejectedCounter++;
            `
        }
    }while(VerifyState != kVerified);    

    */
    (*CurrentStatus) = PackageManagerStatusType::kActivated;

    return ara::ucm::OperationResultType::kSuccess;

}

/*no idea how to impelement*/
ara::ucm::OperationResultType PackageManagerState::Cancel(ara::ucm::TransferIdType id)
{

    if ( (*CurrentStatus) != PackageManagerStatusType::kProcessing)
    {
        return ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    SWPackagesCounter--;

    if (SWPackagesCounter == 0)
    {
        (*CurrentStatus) != PackageManagerStatusType::kIdle;
    }
    else 
    {
        (*CurrentStatus) != PackageManagerStatusType::kReady;
    }

    return ara::ucm::OperationResultType::kSuccess;

}


/*remove process list*/
ara::ucm::OperationResultType PackageManagerState::Finish()
{
    if (((*CurrentStatus) != PackageManagerStatusType::kRolledBack) && ((*CurrentStatus) != PackageManagerStatusType::kActivated))
    {
       return ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    (*CurrentStatus) == PackageManagerStatusType::kCleaningUp;

    /*if UCM status is activated finish method commit changes*/
    if ( (*CurrentStatus) == PackageManagerStatusType::kActivated)
    {
        ara::ucm::storage::SWCLManager::CommitChanges();
    }

    /*if UCM status is krolledback finish method revert changes*/
    if ( (*CurrentStatus) == PackageManagerStatusType::kRolledBack)
    {
        ara::ucm::storage::SWCLManager::RevertChanges();
    }

    /*change UCM status into Kcleaningup*/
    (*CurrentStatus) == PackageManagerStatusType::kIdle;
    SWPackagesCounter=0;
    return ara::ucm::OperationResultType::kSuccess;

}


PackageManagerStatusType PackageManagerState::GetStatus()
{
    return (*CurrentStatus);
}


ara::ucm::OperationResultType PackageManagerState::ProcessSwPackage(TransferIdType &id)
{
    /* ONLY ONE PACKAGE CAN BE PROCESSED AT A TIME AND UCM STATE MUST BE IDLE AND KREADY */
    if ( (*CurrentStatus) != PackageManagerStatusType::kIdle && ((*CurrentStatus) != PackageManagerStatusType::kReady))
    {
       return ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    SWPackagesCounter++;
    /* CURRENT STATUS OF UCM = START PROCESSING */
    (*CurrentStatus) = PackageManagerStatusType::kProcessing;

    /* CREATE A POINTER TO GET THE SOFTWARE PACKAGE THROUGH THE ID */
    shared_ptr<ara::ucm::transfer::SoftwarePackage> ptrToSwPkg = ara::ucm::SynchronizedStorage::GetItem(id);

    /* CHECK FOR CORRECT ID */
    if (ptrToSwPkg == nullptr)
	{
       return ara::ucm::OperationResultType::kInvalidTransferId;
    }

    /* GET PACKAGE */
    string SWPackagePath{ptrToSwPkg->GetPackagePath()} ;

    /* USED TO PARSE */
    ara::ucm::parsing::SoftwarePackageParser SWParser_instance;
    
    /* SWPACKAGEPATH IS UPDATED WITH ITS PATH AFTER UNZIPPING */
    SWPackagePath = SWParser_instance.UnzipPackage(SWPackagePath);
    
    SWParser_instance.SwPackageManifestParser(SWPackagePath);

    SwClusterInfoType NewSwClusterInfo {SWParser_instance.GetSwClusterInfo(SWPackagePath)};

    ActionType action { SWParser_instance.GetActionType() };

    /* OVERRIDE CLASS */
    shared_ptr<storage::ReversibleAction> actionPtr;

    /* CHECK ACTION TYPE AND ACT ACCORDING TO IT */
    if ( action == ActionType::kUpdate )
    {
        actionPtr = make_shared<storage::UpdateAction> (SWPackagePath,NewSwClusterInfo) ;   
    }

    else if ( action == ActionType::kInstall )
    {
        actionPtr = make_shared<storage::InstallAction> (SWPackagePath,NewSwClusterInfo) ;   
    }
    
    else if ( action == ActionType::kRemove )
    {    
        actionPtr = make_shared<storage::RemoveAction> (SWPackagePath,NewSwClusterInfo) ;   
    }

    else
    {
        // WRONG ACTION TYPE
    }

    actionPtr->Execute();
    
    ara::ucm::storage::SWCLManager::AddSWCLChangeInfo(NewSwClusterInfo , actionPtr);

    (*CurrentStatus) = PackageManagerStatusType::kReady;

    ara::ucm::SynchronizedStorage::DeleteItem(id);      
    
    return ara::ucm::OperationResultType::kSuccess;

}


 /* missing KProcessing*/
ara::ucm::OperationResultType PackageManagerState::RevertProcessedSwPackages()
{   
    if ((*CurrentStatus) != PackageManagerStatusType::kReady || (*CurrentStatus) != PackageManagerStatusType::kProcessing)
    {
        return ara::ucm::OperationResultType::kOperationNotPermitted;
    }
   
    if ((*CurrentStatus) == PackageManagerStatusType::kReady)
    {
        (*CurrentStatus) == PackageManagerStatusType::kCleaningUp;

        ara::ucm::storage::SWCLManager::RevertChanges();

        (*CurrentStatus) == PackageManagerStatusType::kIdle;
        SWPackagesCounter=0;

    }
    return ara::ucm::OperationResultType::kSuccess;
}


ara::ucm::OperationResultType PackageManagerState::Rollback()
{

    if ( (*CurrentStatus) != PackageManagerStatusType::kActivated && (*CurrentStatus) != PackageManagerStatusType::kVerifying)
    {
        /* PKG not in Acitvited or Verifing state */
        /*Operation Not Permitted*/
        return ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    (*CurrentStatus) = PackageManagerStatusType::kRollingBack;

    /* Remove New Process List */

    /*

    do
    {
        SM_ReceivedStates RollbackState = SM::PrepareUpdate(functional_Group);



        if(RollbackState == SM_ReceivedStates::kRollbackFailed)
        {
            (*CurrentStatus) = PackageManagerStatusType::kReady;

            NotAbleToRollback

            return
            
        }
        else if (RollbackState == SM_ReceivedStates::kRejected)
        {
            (*CurrentStatus) = PackageManagerStatusType::kReady;

            NotAbleToRollback

            return
        }
    }while(RollbackState != kPrepared); msh 3arfen hn5rog azay
    
    */

    (*CurrentStatus) = PackageManagerStatusType::kRolledBack;

    return ara::ucm::OperationResultType::kSuccess;
}
