#include "includes/Storage.hpp"

/*////////////////////////////////////////////////////////////////////////////

    SWCLManager Class Methods

////////////////////////////////////////////////////////////////////////////*/


map <ara::ucm::storage::ReversibleAction * , ara::ucm::SwClusterInfoType> SWClustersData;

void ara::ucm::storage::SWCLManager::AddSWCLChangeInfo(ara::ucm::SwClusterInfoType NewSWClusterInfo, ara::ucm::storage::ReversibleAction * ReversibleAct)
{
    ara::ucm::storage::SWCLManager::SWClustersData.emplace(ReversibleAct, NewSWClusterInfo);
}

void ara::ucm::storage::SWCLManager::CommitChanges()
{
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if ((itr->second).State!= ara::ucm::SwClusterStateType::kPresent)
        {
            ara::ucm::storage::ReversibleAction * Act = itr -> first;
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
            ara::ucm::storage::ReversibleAction * Act = itr ->first;
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

    InstallAction Class Methods

////////////////////////////////////////////////////////////////////////////*/


void ara::ucm::storage::InstallAction::Execute()
{
    /* Get path of clusters folder */
    std::string command;
    std::string ClustersPath = "clusters_path"; //hayb2a path mawgod 
    std::string swClusterPath = ClustersPath + "/cluster_name/"; //assuming i have the cluster name
    /* Check first if path exists, if not create the folder ZIP_Packages*/
    if (!IsPathExist(swClusterPath.c_str()))
    {
        command = "mkdir " + swClusterPath;
        system(command.c_str());
    }
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kAdded);
}

void ara::ucm::storage::InstallAction::CommitChanges()
{
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kPresent);
}

void ara::ucm::storage::InstallAction::RevertChanges()
{
    // Remove the directory made by Install
    std::string SWDirectoryPath = "path";
    std::string command;
    if (IsPathExist(SWDirectoryPath.c_str()))
    {
        command = "rm -r " + SWDirectoryPath;
        system(command.c_str());
    }
    /*ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kPresent);*/
}



/*////////////////////////////////////////////////////////////////////////////

    RemoveAction Class Methods

////////////////////////////////////////////////////////////////////////////*/


void ara::ucm::storage::RemoveAction::Execute()
{
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kRemoved);
}

void ara::ucm::storage::RemoveAction::CommitChanges()
{
    // Remove the directory made by Install
    std::string SWDirectoryPath = "3abat";
    std::string command;
    if(IsPathExist(SWDirectoryPath.c_str()))
    {
        command = "rm -r " + SWDirectoryPath;
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


void ara::ucm::storage::UpdateAction::Execute()
{
    string command;
    /* GET CURRENT WORKING DIRECTORY PATH */
    string ProjectPath = GetCurrentDirectory();
    chdir(ProjectPath.c_str());

    /* GET THE PACKAGE */
    string ClusterPath =  ProjectPath + "\\ZIP_Packages\\ TID"  ;   

    /* GET THE PATH TO THE FILE SYSTEM DIRECTORY AND CHECK IF IT DOESN'T EXIST */
    std::string SWDirectoryPath = "?\\FileSystemPath\\SWNAME";
    if (IsPathExist(SWDirectoryPath.c_str()) == false )
    {
        command = "mkdir " + SWDirectoryPath;
        system(command.c_str());
    }

    /* MOVE THE PACKAGE CONTENTS TO THE FILE SYSTEM DIRECTORY */
    /* -b HERE IS A UNIX COMMAND THAT WILL CREATE A BACKUP FILE THAT WILL BE OVERWRITTEN AS A RESULT  */
    std::string command = "mv -b "+ ClusterPath + " " + SWDirectoryPath;   
    system(command.c_str());
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kUpdated);
    
}

void ara::ucm::storage::UpdateAction::CommitChanges()
{
     /* NAME AND PATH OF BACKUP FILE */
    string SWDirectoryPathBackUp = "?\\FileSystemPath\\SWNAME~";   
    string command;
    if (IsPathExist(SWDirectoryPathBackUp.c_str()) == true ) 
    {
        command = "rm -r " + SWDirectoryPathBackUp;
    }

    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kPresent);
}

void ara::ucm::storage::UpdateAction::RevertChanges()
{
    string SWDirectoryPath = "?\\FileSystemPath\\SWNAME";
    /* NAME AND PATH OF BACKUP FILE */
    string SWDirectoryPathBackUp = "?\\FileSystemPath\\SWNAME~";
    string command;
    if(IsPathExist(SWDirectoryPath.c_str()))
    {
        command = "rm -r " + SWDirectoryPath;
        system(command.c_str());
        /* RENAME BACKUP WITH ITS ORIGINAL NAME (REVERT) */
        command = "mv " + SWDirectoryPathBackUp + " " + SWDirectoryPath;
    }

    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kPresent);
}


