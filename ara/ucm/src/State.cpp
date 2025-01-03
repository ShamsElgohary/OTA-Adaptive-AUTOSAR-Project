#include "../includes/State.hpp"
#include "../includes/Storage.hpp"
#include "../includes/SynchronizedStorage.hpp"
#include "../includes/Parsing.hpp"
#include "../includes/UCM_Common.hpp"

using namespace ara::ucm::storage;
using namespace ara::ucm::state;
using namespace ara::sm::UpdateRequest::proxy;
using namespace ara::com::proxy;

uint16_t PackageManagerState::ProcessListVersion = 0;

PackageManagerState::PackageManagerState(PackageManagerStatusType pkgmgr_CurrentStatus)
{
    /* CURRENT STATUS IS REFERENCED TO THE STATUS IN THE INTERFACE PACKAGEMANAGER */
    this->CurrentStatus = pkgmgr_CurrentStatus;

    json CurrentProcessList;
    std::ifstream JsonInStream(ProcessListPath + "Process_List.json");
    try
    {
        JsonInStream >> CurrentProcessList;
        PackageManagerState::ProcessListVersion = CurrentProcessList["Process List Version"];
        json::iterator it = CurrentProcessList.begin();
        for (; it != CurrentProcessList.end(); ++it)
        {
            if (it.key() == "Process List Version")
            {
                continue;
            }
            ara::ucm::SwClusterInfoType SWCluster;
            SWCluster.Name = it.key();
            SWCluster.Version = CurrentProcessList[it.key()]["Version"];
            SWCluster.State = ara::ucm::SwClusterStateType::kPresent;

            SWCLManager::PushInSWCLusters(SWCluster);
        }
    
    GUI_Logger.ReportPresentSWClusters(SWCLManager::GetPresentSWCLs());
    
    }
    catch (const std::exception &e)
    {
        std::cout << "[UCM] ERROR Catch ProcessList: " << e.what() << std::endl;
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

ara::ucm::OperationResultType PackageManagerState::ActivateInternal()
{
    ara::com::ServiceHandleContainer<UpdateRequestProxy::HandleType> handles_container = ara::sm::UpdateRequest::proxy::UpdateRequestProxy::FindService();

    if (handles_container.size() == 0)
    {   
        cout<<"[ucm] No SM Handle Found"<<endl;
        return ara::ucm::OperationResultType::kOperationNotPermitted;
    }
    else
    {
        cout<<"[ucm] SM Handle Found"<<endl;
        proxy = std::make_shared<UpdateRequestProxy>(handles_container[0]);
    }

    if ((CurrentStatus) != PackageManagerStatusType::kReady)
    {
        /* PKG not in ready state */
        /*Operation Not Permitted*/
        return ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    StartUpdateSessionOutput error_startUpdateSession = proxy->StartUpdateSession();
    if (error_startUpdateSession.AppError == SM_ApplicationError::kRejected)
    {
        GUI_Logger.ReportJsonGUI( "StartUpdateSession", "kRejected", true );
        return ara::ucm::OperationResultType::kUpdateSessionRejected;
    }

    GUI_Logger.ReportJsonGUI( "StartUpdateSession", "Done", true );

    uint8_t RejectedCounter = 0;
    /*Get Functional Groups of PKG*/
    FunctionGroupList FG_List = {"fn1"};

    int PrepareTrials = 0;
    PrepareUpdateOutput error_prepareUpdate = proxy->PrepareUpdate(FG_List);
    PrepareTrials += 1;
    while ((error_prepareUpdate.AppError != SM_ApplicationError::kPrepared) && (PrepareTrials < 5))
    {
        error_prepareUpdate = proxy->PrepareUpdate(FG_List);

        if (error_prepareUpdate.AppError == SM_ApplicationError::kPrepareFailed)
        {
            (CurrentStatus) = PackageManagerStatusType::kReady;

            GUI_Logger.ReportJsonGUI( "PrepareUpdate", "Prepare Failed", true );

            return ara::ucm::OperationResultType::kPreActivationFailed;
        }
        else if (error_prepareUpdate.AppError == SM_ApplicationError::kRejected)
        {
            if (RejectedCounter == PrepareUpdateCounter)
            {
                (CurrentStatus) = PackageManagerStatusType::kReady;

                GUI_Logger.ReportJsonGUI( "PrepareUpdate", "Rejected" , true );
                return ara::ucm::OperationResultType::kPreActivationFailed;
            }

            RejectedCounter++;
        }
        PrepareTrials += 1;
    }

    GUI_Logger.ReportJsonGUI( "PrepareUpdate", "Prepared" , true );


    (CurrentStatus) = PackageManagerStatusType::kActivating;
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
    for (auto itr = SwChangedClusters.begin(); itr != SwChangedClusters.end(); ++itr)
    {
        if (((itr->State) == ara::ucm::SwClusterStateType::kUpdated) || ((itr->State) == ara::ucm::SwClusterStateType::kAdded))
        {
            NewProListSwClusters.push_back(*itr);
        }
    }
    /* CREATE JSON OBJECT FOR PROCESS LIST */
    json processList;

    PackageManagerState::ProcessListVersion = PackageManagerState::ProcessListVersion + 1;

    processList["Process List Version"] = PackageManagerState::ProcessListVersion;

    for (auto itr = NewProListSwClusters.begin(); itr != NewProListSwClusters.end(); ++itr)
    {
        std::string Path{"/" + itr->Name + "/" + itr->Version + "/"};

        processList[itr->Name] = {{"Version", itr->Version}, {"Path", Path}};
    }
    /* MOVE OLD PROCESS LIST TO BACKUP FOLDER */
    command = "mv " + ProcessListPath + "Process_List.json " + ProcessListPath + "Backup/";
    system(command.c_str());
    /* GENERATE THE NEW PROCESS LIST FILE */
    std::ofstream JsonOutStream(ProcessListPath + "Process_List.json");
    JsonOutStream << std::setw(4) << processList << std::endl;

    /*Depenency Missing Error Check*/
    DependencyCheck();

    RejectedCounter = 0;

    int VerifyTrails = 0;
    VerifyUpdateOutput error_verifyupdate = proxy->VerifyUpdate(FG_List);
    VerifyTrails += 1;
    while ((error_verifyupdate.AppError != SM_ApplicationError::kVerified) && (VerifyTrails < 5))
    {
        error_verifyupdate = proxy->VerifyUpdate(FG_List);

        (CurrentStatus) = PackageManagerStatusType::kVerifying;

        if (error_verifyupdate.AppError == SM_ApplicationError::kVerifyFailed)
        {
            (CurrentStatus) = PackageManagerStatusType::kReady;


            GUI_Logger.ReportJsonGUI( "VerifyUpdate", "Verify Failed", true );
            return ara::ucm::OperationResultType::kVerificationFailed;
        }
        else if (error_verifyupdate.AppError == SM_ApplicationError::kRejected)
        {
            if (RejectedCounter == PrepareUpdateCounter)
            {
                // Rollback();
                GUI_Logger.ReportJsonGUI( "VerifyUpdate", "Rejected", true );
                return ara::ucm::OperationResultType::kVerificationFailed;
            }
            RejectedCounter++;
        }
        VerifyTrails += 1;
    }

    GUI_Logger.ReportJsonGUI( "VerifyUpdate", "Verified", true );
    GUI_Logger.ReportPresentSWClusters(SWCLManager::GetPresentSWCLs());

    (CurrentStatus) = PackageManagerStatusType::kActivated;
    return ara::ucm::OperationResultType::kSuccess;
}

/*no idea how to impelement*/
ara::ucm::OperationResultType PackageManagerState::Cancel(ara::ucm::TransferIdType id)
{

    if ((CurrentStatus) != PackageManagerStatusType::kProcessing)
    {
        return ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    SWPackagesCounter--;

    if (SWPackagesCounter == 0)
    {
        (CurrentStatus) != PackageManagerStatusType::kIdle;
    }
    else
    {
        (CurrentStatus) != PackageManagerStatusType::kReady;
    }

    return ara::ucm::OperationResultType::kSuccess;
}

/*remove process list*/
ara::ucm::OperationResultType PackageManagerState::FinishInternal()
{
    if (((CurrentStatus) != PackageManagerStatusType::kRolledBack) && ((CurrentStatus) != PackageManagerStatusType::kActivated))
    {
        return ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    (CurrentStatus) = PackageManagerStatusType::kCleaningUp;

    proxy->StopUpdateSession();
    GUI_Logger.ReportJsonGUI( "StopUpdateSession", "Stopped", true );

    /*if UCM status is activated finish method commit changes*/
    if ((CurrentStatus) == PackageManagerStatusType::kActivated)
    {
        ara::ucm::storage::SWCLManager::CommitChanges();
    }

    /*if UCM status is krolledback finish method revert changes*/
    if ((CurrentStatus) == PackageManagerStatusType::kRolledBack)
    {
        ara::ucm::storage::SWCLManager::RevertChanges();
    }

    command = "rm " + fileSystemPath + "/etc/system/" + "Backup/*";
    system(command.c_str());

    // command = "rm " + ProcessListPath + "Backup/Process_List.json";
    // system(command.c_str());
    SWCLManager::PresentSWCLChangeInfo();
    json CurrentProcessList;
    std::ifstream JsonInStream(ProcessListPath + "Process_List.json");
    try
    {
        
        JsonInStream >> CurrentProcessList;
        PackageManagerState::ProcessListVersion = CurrentProcessList["Process List Version"];
        json::iterator it = CurrentProcessList.begin();
        for (; it != CurrentProcessList.end(); ++it)
        {
            if (it.key() == "Process List Version")
            {
                continue;
            }
            ara::ucm::SwClusterInfoType SWCluster;
            SWCluster.Name = it.key();
            SWCluster.Version = CurrentProcessList[it.key()]["Version"];
            SWCluster.State = ara::ucm::SwClusterStateType::kPresent;
            cout<<SWCluster.Name<<endl;
            SWCLManager::PushInSWCLusters(SWCluster);
            
        }
    
    GUI_Logger.ReportPresentSWClusters(SWCLManager::GetPresentSWCLs());
    
    }
    catch (const std::exception &e)
    {
        std::cout << "[UCM] ERROR Catch ProcessList: " << e.what() << std::endl;
        PackageManagerState::ProcessListVersion = 0;
    }

    /*change UCM status into Kcleaningup*/
    (CurrentStatus) = PackageManagerStatusType::kIdle;
    SWPackagesCounter = 0;
    return ara::ucm::OperationResultType::kSuccess;
}

PackageManagerStatusType PackageManagerState::GetStatusInternal()
{
    return (CurrentStatus);
}

ara::ucm::OperationResultType PackageManagerState::ProcessSwPackageInternal(TransferIdType id)
{
    /* ONLY ONE PACKAGE CAN BE PROCESSED AT A TIME AND UCM STATE MUST BE IDLE AND KREADY */
    if ((CurrentStatus) != PackageManagerStatusType::kIdle && ((CurrentStatus) != PackageManagerStatusType::kReady))
    {
        return ara::ucm::OperationResultType::kOperationNotPermitted;
    }
    SWPackagesCounter++;
    /* CURRENT STATUS OF UCM = START PROCESSING */
    (CurrentStatus) = PackageManagerStatusType::kProcessing;

    /* CREATE A POINTER TO GET THE SOFTWARE PACKAGE THROUGH THE ID */
    shared_ptr<ara::ucm::transfer::SoftwarePackage> ptrToSwPkg = ara::ucm::SynchronizedStorage::GetItem(id);

    /* CHECK FOR CORRECT ID */
    if (ptrToSwPkg == nullptr)
    {
        return ara::ucm::OperationResultType::kInvalidTransferId;
    }

    /* CHANGE STATE TO KPROCESSING */
    ptrToSwPkg->SetPackageState(SwPackageStateType::kProcessing);

    // JSON (GUI SIMULATION)
    GUI_Logger.pkgAction("kProcessing",1);

    /* GET PACKAGE */
    std::string SWPackagePath{ptrToSwPkg->GetPackagePath()};
    /* USED TO PARSE */
    ara::ucm::parsing::SoftwarePackageParser SWParser_instance;

    /* SWPACKAGEPATH IS UPDATED WITH ITS PATH AFTER UNZIPPING */
    SWPackagePath = SWParser_instance.UnzipPackage(SWPackagePath);

    SWParser_instance.SwPackageManifestParser(SWPackagePath);
    /* GET SW CLUSTER INFORMATION FROM PARSING */
    SwClusterInfoType NewSwClusterInfo{SWParser_instance.GetSwClusterInfo(SWPackagePath)};
    /* GET SW ACTION TYPE FROM PARSING */
    ActionType action{SWParser_instance.GetActionType()};

    shared_ptr<ara::ucm::storage::ReversibleAction> actionPtr;

    /* ADD SWCLUSTER & EXECUTE */
    actionPtr = SWCLManager::AddSWCLChangeInfo(NewSwClusterInfo, action, SWPackagePath);


    (CurrentStatus) = PackageManagerStatusType::kReady;

    /* CHANGE STATE TO KPROCESSING */
    ptrToSwPkg->SetPackageState(SwPackageStateType::kProcessed);


    // JSON (GUI SIMULATION)
    GUI_Logger.pkgAction("kProcessed",1);

    ara::ucm::SynchronizedStorage::DeleteItem(id);

    return ara::ucm::OperationResultType::kSuccess;
}

/* missing KProcessing*/
ara::ucm::OperationResultType PackageManagerState::RevertProcessedSwPackages()
{
    if ((CurrentStatus) != PackageManagerStatusType::kReady || (CurrentStatus) != PackageManagerStatusType::kProcessing)
    {
        return ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    if ((CurrentStatus) == PackageManagerStatusType::kReady)
    {
        (CurrentStatus) = PackageManagerStatusType::kCleaningUp;

        ara::ucm::storage::SWCLManager::RevertChanges();

        (CurrentStatus) = PackageManagerStatusType::kIdle;
        SWPackagesCounter = 0;
    }
    return ara::ucm::OperationResultType::kSuccess;
}

ara::ucm::OperationResultType PackageManagerState::RollbackInternal()
{

    if ((CurrentStatus) != PackageManagerStatusType::kActivated && (CurrentStatus) != PackageManagerStatusType::kVerifying)
    {
        /* PKG not in Acitvited or Verifing state */
        /*Operation Not Permitted*/
        return ara::ucm::OperationResultType::kOperationNotPermitted;
    }

    (CurrentStatus) = PackageManagerStatusType::kRollingBack;

    /* REMOVE NEW PROCESS LIST */
    command = "rm " + ProcessListPath + "Process_List.json";
    system(command.c_str());

    /* RETURN OLD PROCESS LIST FROM BACKUP */
    command = "mv " + ProcessListPath + "/Backup/Process_List.json " + ProcessListPath;
    system(command.c_str());

    /*

    do
    {
        SM_ReceivedStates RollbackState = SM::PrepareUpdate(functional_Group);



        if(RollbackState == SM_ReceivedStates::kRollbackFailed)
        {
            (CurrentStatus) = PackageManagerStatusType::kReady;

            NotAbleToRollback

            return

        }
        else if (RollbackState == SM_ReceivedStates::kRejected)
        {
            (CurrentStatus) = PackageManagerStatusType::kReady;

            NotAbleToRollback

            return
        }
    }while(RollbackState != kPrepared); msh 3arfen hn5rog azay

    */

    (CurrentStatus) = PackageManagerStatusType::kRolledBack;

    return ara::ucm::OperationResultType::kSuccess;
}