#include "includes/Storage.hpp"


bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

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