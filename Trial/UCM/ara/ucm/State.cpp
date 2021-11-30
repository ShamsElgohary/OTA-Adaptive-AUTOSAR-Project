#include "includes/State.hpp"

using namespace ara::ucm::storage;
using namespace ara::ucm::state;

void PackageManagerState::dependencyCheck(void)
{

}

void PackageManagerState::Activate()
{

    if (PackageManagerState::CurrentStatus != PackageManagerStatusType::kReady)
    {
        /* PKG not in ready state */
        /*Operation Not Permitted*/
        return;
    }

    PackageManagerState::CurrentStatus = PackageManagerStatusType::kActivating;
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
            PackageManagerState::CurrentStatus = PackageManagerStatusType::kReady;

            PreActivation Failed Error

            return
            
        }
        else if (PrepareState == SM_ReceivedStates::kRejected)
        {
            if(RejectedCounter == 6 (hash define for rejected counter))
            {
                PackageManagerState::CurrentStatus = PackageManagerStatusType::kReady;

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
        PackageManagerState::CurrentStatus = PackageManagerStatusType::kVerifing;


        if(VerifyState == SM_ReceivedStates::kVerifyFailed)
        {
            PackageManagerState::CurrentStatus = PackageManagerStatusType::kReady;

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
            
        }
    }while(VerifyState != kVerified);    

    */
    PackageManagerState::CurrentStatus = PackageManagerStatusType::kActivated;


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
void PackageManagerState::ProcessSwPackage(ara::ucm::TransferIdType id)
{
}
void PackageManagerState::RevertProcessedSwPackages()
{
}
void PackageManagerState::Rollback()
{


    if (PackageManagerState::CurrentStatus != PackageManagerStatusType::kActivated || PackageManagerState::CurrentStatus != PackageManagerStatusType::kVerifying)
    {
        /* PKG not in Acitvited or Verifing state */
        /*Operation Not Permitted*/
        return;
    }

    PackageManagerState::CurrentStatus = PackageManagerStatusType::kRollingBack;

    /* Remove New Process List */

    /*

    do
    {
        SM_ReceivedStates RollbackState = SM::PrepareUpdate(functional_Group);



        if(RollbackState == SM_ReceivedStates::kRollbackFailed)
        {
            PackageManagerState::CurrentStatus = PackageManagerStatusType::kReady;

            NotAbleToRollback

            return
            
        }
        else if (RollbackState == SM_ReceivedStates::kRejected)
        {
            PackageManagerState::CurrentStatus = PackageManagerStatusType::kReady;

            NotAbleToRollback

            return
        }
    }while(RollbackState != kPrepared); msh 3arfen hn5rog azay
    
    */

       PackageManagerState::CurrentStatus = PackageManagerStatusType::kRolledBack;
}
