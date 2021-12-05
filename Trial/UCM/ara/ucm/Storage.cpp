#include "includes/Storage.hpp"

using namespace ara::ucm;

/*////////////////////////////////////////////////////////////////////////////

    SWCLManager Class Methods

////////////////////////////////////////////////////////////////////////////*/


map < shared_ptr<ara::ucm::storage::ReversibleAction > , ara::ucm::SwClusterInfoType> storage::SWCLManager::SWClustersData;


void ara::ucm::storage::SWCLManager::AddSWCLChangeInfo(SwClusterInfoType NewSWClusterInfo, shared_ptr<storage::ReversibleAction> ReversibleAct)
{
    SWClustersData.emplace(ReversibleAct, NewSWClusterInfo);
}

void ara::ucm::storage::SWCLManager::CommitChanges()
{
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if ((itr->second).State!= ara::ucm::SwClusterStateType::kPresent)
        {
            shared_ptr<ara::ucm::storage::ReversibleAction>  Act = itr -> first;
            Act -> CommitChanges();
        }
    }
}

vector <ara::ucm::SwClusterInfoType> ara::ucm::storage::SWCLManager::GetPresentSWCLs()
{
    // Returns all the Clusters that are in Present State (kPresent)
    vector <ara::ucm::SwClusterInfoType> PresentSWClusters;
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if ((itr->second).State == ara::ucm::SwClusterStateType::kPresent)
        {
            PresentSWClusters.push_back(itr->second);
        }
    }
    return PresentSWClusters;
}

vector <ara::ucm::SwClusterInfoType> ara::ucm::storage::SWCLManager::GetSWCLsChangeInfo()
{
    // Returns all the Clusters that has any changes (kAdded/kRemoved/kUpdated)
    vector <ara::ucm::SwClusterInfoType> ChangeSWClusters;
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if ((itr->second).State != ara::ucm::SwClusterStateType::kPresent)
        {
            ChangeSWClusters.push_back(itr->second);
        }
    }
    return ChangeSWClusters;
}

void ara::ucm::storage::SWCLManager::ResetSWCLChangeInfo()
{
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if ((itr->second).State == ara::ucm::SwClusterStateType::kAdded)
        {
            SWClustersData.erase(itr);
        }
        else if (((itr->second).State == ara::ucm::SwClusterStateType::kRemoved) || ((itr->second).State  == ara::ucm::SwClusterStateType::kUpdated))
        {
            (itr -> second).State = ara::ucm::SwClusterStateType::kPresent;
        }
    }
}

void ara::ucm::storage::SWCLManager::RevertChanges()
{
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if ((itr->second).State  != ara::ucm::SwClusterStateType::kPresent)
        {
            shared_ptr<ara::ucm::storage::ReversibleAction> Act = itr ->first;
            Act -> RevertChanges();
        }
    }
}

void ara::ucm::storage::SWCLManager::SetSWCLState(ara::ucm::SwClusterInfoType ChangedSWCluster, ara::ucm::SwClusterStateType NewState)
{
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if (((itr->second).Name == ChangedSWCluster.Name) && ((itr->second).Version == ChangedSWCluster.Version))
        {
            (itr->second).State = NewState;
        }
    }
}

void ara::ucm::storage::SWCLManager::RemoveSWCL(ara::ucm::SwClusterInfoType ChangedSWCluster)
{
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if (((itr->second).Name == ChangedSWCluster.Name) && ((itr->second).Version == ChangedSWCluster.Version))
        {
            SWClustersData.erase(itr);
        }
    }
}



/*////////////////////////////////////////////////////////////////////////////

    ReversibleAction Class Constructor

////////////////////////////////////////////////////////////////////////////*/

ara::ucm::storage::ReversibleAction::ReversibleAction(string swPackagePath, ara::ucm::SwClusterInfoType SwClusterInfo)
{
    this-> swPackagePath = swPackagePath;
    this-> SwClusterInfo = SwClusterInfo;
}


/*////////////////////////////////////////////////////////////////////////////

    InstallAction Class Methods

////////////////////////////////////////////////////////////////////////////*/

/* CONSTRUCTOR */
ara::ucm::storage::InstallAction::InstallAction(string swPackagePath, SwClusterInfoType SwClusterInfo): ReversibleAction(swPackagePath,SwClusterInfo)
{}


void ara::ucm::storage::InstallAction::Execute()
{
    /* GET THE SW CLUSTER INSIDE THE SW PACKAGE  */
    string clusterTransferPath { this->swPackagePath + "/SoftwareCluster/" + this->SwClusterInfo.Name + "/*" };   

    /* GET THE PATH TO THE FILE SYSTEM DIRECTORY */
    string clusterInFileSystem { fileSystemPath + "/" + this->SwClusterInfo.Name }; 
    
    command = "mkdir " + clusterInFileSystem;
    system(command.c_str());

    /* ADD VERSION TO THE  PATH IN THE FILE SYSTEM DIRECTORY */
    clusterInFileSystem = clusterInFileSystem + "/" + this->SwClusterInfo.Version ; 

    command = "mkdir " + clusterInFileSystem;
    system(command.c_str());

    /* MOVE THE PACKAGE CONTENTS TO THE FILE SYSTEM DIRECTORY */
    command = "mv "+ clusterTransferPath + " " + clusterInFileSystem;   
    system(command.c_str());

    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kAdded);
}

void ara::ucm::storage::InstallAction::CommitChanges()
{
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kPresent);
}

void ara::ucm::storage::InstallAction::RevertChanges()
{
    /* GET THE PATH TO THE FILE SYSTEM DIRECTORY ( VERSION NEEDED ) */
    string clusterInFileSystem { fileSystemPath + "/" + this->SwClusterInfo.Name + "/" + this->SwClusterInfo.Version }; 

    if (IsPathExist(clusterInFileSystem.c_str()))
    {
        /* REMOVE INSTALLED SW CLUSTER*/
        command = "rm -r " + clusterInFileSystem;
        system(command.c_str());
    }

}



/*////////////////////////////////////////////////////////////////////////////

    RemoveAction Class Methods

////////////////////////////////////////////////////////////////////////////*/

/* CONSTRUCTOR */
ara::ucm::storage::RemoveAction::RemoveAction(string swPackagePath, SwClusterInfoType SwClusterInfo): ReversibleAction(swPackagePath,SwClusterInfo)
{}

void ara::ucm::storage::RemoveAction::Execute()
{
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kRemoved);
}

void ara::ucm::storage::RemoveAction::CommitChanges()
{
    /* GET THE PATH TO THE FILE SYSTEM DIRECTORY ( VERSION NEEDED ) */
    string clusterInFileSystem { fileSystemPath + "/" + this->SwClusterInfo.Name + "/" + this->SwClusterInfo.Version }; 

    if (IsPathExist(clusterInFileSystem.c_str()))
    {
        /* MOVE TO THE BACKUP FOLDER */
        command = "rm -r " + clusterInFileSystem;
        system(command.c_str());
    }
}

void ara::ucm::storage::RemoveAction::RevertChanges()
{
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kPresent);
}


/*////////////////////////////////////////////////////////////////////////////

    UpdateAction Class Methods

////////////////////////////////////////////////////////////////////////////*/


/* CONSTRUCTOR */
ara::ucm::storage::UpdateAction::UpdateAction(string swPackagePath, SwClusterInfoType SwClusterInfo): ReversibleAction(swPackagePath,SwClusterInfo)
{}

void ara::ucm::storage::UpdateAction::Execute()
{

    /* GET THE SW CLUSTER INSIDE THE SW PACKAGE  */
    string clusterTransferPath { this->swPackagePath + "/SoftwareCluster/" + this->SwClusterInfo.Name + "/*" };   

    /* GET THE PATH TO THE FILE SYSTEM DIRECTORY */
    string clusterInFileSystem { fileSystemPath + "/" + this->SwClusterInfo.Name }; 

    /* SEND SW CLUSTER (OLDER VERSION) TO THE BACKUP INCASE OF ROLL BACK */
    if (IsPathExist(clusterInFileSystem.c_str()) )
    {
    command = "mv "+ clusterInFileSystem + " " + fileBackupPath;   
    system(command.c_str());
    }
    
    command = "mkdir " + clusterInFileSystem;
    system(command.c_str());

    /* ADD VERSION TO THE  PATH IN THE FILE SYSTEM DIRECTORY */
    clusterInFileSystem = clusterInFileSystem + "/" + this->SwClusterInfo.Version ; 

    command = "mkdir " + clusterInFileSystem;
    system(command.c_str());

    /* MOVE THE PACKAGE CONTENTS TO THE FILE SYSTEM DIRECTORY */
    command = "mv "+ clusterTransferPath + " " + clusterInFileSystem;   
    system(command.c_str());
    
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kUpdated);
}


void ara::ucm::storage::UpdateAction::CommitChanges()
{
    string clusterInBackup { fileBackupPath + "/" + this->SwClusterInfo.Name }; 
    
    /* REMOVE THE OLD VERSION OF THE SW CLUSTER IN BACKUP */
    if ( IsPathExist(clusterInBackup.c_str()) )
    {
    command = "rm -r " + clusterInBackup;
    system(command.c_str());
    }

    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kPresent);
}


void ara::ucm::storage::UpdateAction::RevertChanges()
{
    /* GET THE PATH TO THE FILE SYSTEM DIRECTORY ( VERSION NEEDED ) */
    string clusterInFileSystem { fileSystemPath + "/" + this->SwClusterInfo.Name }; 
    /* GET PATH OF OLD SW CLUSTER IN BACKUP */
    string clusterInBackup { fileBackupPath + "/" + this->SwClusterInfo.Name }; 

    if(IsPathExist(clusterInFileSystem.c_str()))
    {
        command = "rm -r " + clusterInFileSystem;
        system(command.c_str());
    }

    /* MOVE THE PACKAGE CONTENTS TO THE FILE SYSTEM DIRECTORY */
    command = "mv "+ clusterInBackup + " " + fileSystemPath;   
    system(command.c_str());

    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kPresent);
}

