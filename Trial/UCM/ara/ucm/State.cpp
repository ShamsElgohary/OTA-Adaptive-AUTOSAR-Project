#include "includes/State.hpp"

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

void PackageManagerState::Activate()
{

    if ((*CurrentStatus) != PackageManagerStatusType::kReady)
    {
        /* PKG not in ready state */
        /*Operation Not Permitted*/
        return;
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


}
void PackageManagerState::Cancel(ara::ucm::TransferIdType id)
{
}

void PackageManagerState::Finish()
{
}
void PackageManagerState::GetStatus()
{
}

ara::ucm::OperationResultType PackageManagerState::ProcessSwPackage(TransferIdType &id)
{
    /* ONLY ONE PACKAGE CAN BE PROCESSED AT A TIME AND UCM STATE MUST BE IDLE */
    if ( (*CurrentStatus) != PackageManagerStatusType::kIdle )
    {
       return ara::ucm::OperationResultType::kOperationNotPermitted;
    }

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
    
    return ara::ucm::OperationResultType::kSuccess;
}



void PackageManagerState::RevertProcessedSwPackages()
{

}

void PackageManagerState::Rollback()
{


    if ((*CurrentStatus) != PackageManagerStatusType::kActivated || (*CurrentStatus) != PackageManagerStatusType::kVerifying)
    {
        /* PKG not in Acitvited or Verifing state */
        /*Operation Not Permitted*/
        return;
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
}
