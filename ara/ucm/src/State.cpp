#include "../includes/State.hpp"
#include "../includes/Storage.hpp"
#include "../includes/SynchronizedStorage.hpp"
#include "../includes/Parsing.hpp"

using namespace ara::ucm::storage;
using namespace ara::ucm::state;

uint16_t PackageManagerState::ProcessListVersion = 0;

PackageManagerState::PackageManagerState(PackageManagerStatusType &pkgmgr_CurrentStatus)
{
    /* CURRENT STATUS IS REFERENCED TO THE STATUS IN THE INTERFACE PACKAGEMANAGER */
    this->CurrentStatus = &pkgmgr_CurrentStatus;

    json CurrentProcessList;
    std::ifstream JsonInStream (fileSystemPath + "/" + "Process_List.json");
    try
    {
        JsonInStream >> CurrentProcessList;
        PackageManagerState::ProcessListVersion = CurrentProcessList["Process List Version"];
        json::iterator it = CurrentProcessList.begin();
        ++it;
        for ( ;it != CurrentProcessList.end(); ++it) 
        {
            ara::ucm::SwClusterInfoType SWCluster;
            SWCluster.Name = it.key();
            SWCluster.Version = CurrentProcessList[it.key()]["Version"];
            SWCluster.State = ara::ucm::SwClusterStateType::kPresent;

            SWCLManager::PushInSWCLusters(SWCluster);
        }

    }
    catch(const std::exception& e)
    {
        PackageManagerState::ProcessListVersion = 0;
    }
}

/* DEFAULT CONSTRUCTOR */
PackageManagerState::PackageManagerState()
{

}


void PackageManagerState::DependencyCheck(void)
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

    /* PROCESSES IN NEW PROCESS LIST */
    vector<ara::ucm::SwClusterInfoType> NewProListSwClusters;

    /* GET UNCHANGED SW CLUSTERS INFORMATION */
    NewProListSwClusters = SWCLManager::GetPresentSWCLs();

    /* ALL CHANGED PROCESSES */
    vector<ara::ucm::SwClusterInfoType> SwChangedClusters;

    /* GET ALL CHANGED PROCESSES */
    SwChangedClusters = SWCLManager::GetSWCLsChangeInfo();

    /* SELECT UPDATED OR INSTALLED SW CLUSTERS ONLY */
    for(auto itr = SwChangedClusters.begin(); itr != SwChangedClusters.end(); ++itr)
    {
        if (((itr->State) == ara::ucm::SwClusterStateType::kUpdated) || ((itr->State) == ara::ucm::SwClusterStateType::kAdded) )
        {
            NewProListSwClusters.push_back(*itr);
        }
    }

    /* CREATE JSON OBJECT FOR PROCESS LIST */
    json processList;

    PackageManagerState::ProcessListVersion = PackageManagerState::ProcessListVersion + 1;

    processList["Process List Version"] =  PackageManagerState::ProcessListVersion;

    for(auto itr = NewProListSwClusters.begin(); itr != NewProListSwClusters.end(); ++itr)
    {
        std::string Path {"/" + itr->Name + "/" + itr->Version + "/"};

        processList[itr->Name] = { {"Version", itr->Version}, {"Path", Path} };
    }

    /* MOVE OLD PROCESS LIST TO BACKUP FOLDER */
    command = "mv "+ fileSystemPath + "/" + "Process_List.json" + " " + fileSystemPath + "/" + "Backup/";   
    system(command.c_str());

    /* GENERATE THE NEW PROCESS LIST FILE */
    std::ofstream JsonOutStream(fileSystemPath + "/" + "Process_List.json");
    JsonOutStream << std::setw(4) << processList << std::endl;

    /*Depenency Missing Error Check*/
    DependencyCheck();

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

    command = "rm "+ fileSystemPath + "/" + "Backup/*";   
    system(command.c_str());

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
    /* CHANGE STATE TO KPROCESSING */
    ptrToSwPkg -> SetPackageState(SwPackageStateType::kProcessing);

    /* GET PACKAGE */
    std::string SWPackagePath{ptrToSwPkg->GetPackagePath()} ;

    /* USED TO PARSE */
    ara::ucm::parsing::SoftwarePackageParser SWParser_instance;
    
    /* SWPACKAGEPATH IS UPDATED WITH ITS PATH AFTER UNZIPPING */
    SWPackagePath = SWParser_instance.UnzipPackage(SWPackagePath);
    
    SWParser_instance.SwPackageManifestParser(SWPackagePath);

    /* GET SW CLUSTER INFORMATION FROM PARSING */
    SwClusterInfoType NewSwClusterInfo {SWParser_instance.GetSwClusterInfo(SWPackagePath)};

    /* GET SW ACTION TYPE FROM PARSING */
    ActionType action { SWParser_instance.GetActionType() };

    shared_ptr<ara::ucm::storage::ReversibleAction > actionPtr;
    
    /* ADD SWCLUSTER & EXECUTE */
    actionPtr = SWCLManager::AddSWCLChangeInfo(NewSwClusterInfo, action, SWPackagePath);

    (*CurrentStatus) = PackageManagerStatusType::kReady;

    /* CHANGE STATE TO KPROCESSING */
    ptrToSwPkg -> SetPackageState(SwPackageStateType::kProcessed);


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

    /* REMOVE NEW PROCESS LIST */
    command = "rm "+ fileSystemPath + "/" + "Process_List.json";   
    system(command.c_str());

    /* RETURN OLD PROCESS LIST FROM BACKUP */
    command = "mv "+ fileSystemPath + "/Backup/Process_List.json" + " " + fileSystemPath + "/";   
    system(command.c_str());


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
