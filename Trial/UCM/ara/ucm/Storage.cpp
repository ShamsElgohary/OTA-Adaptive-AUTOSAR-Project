#include "includes/Storage.hpp"

// Contains all the Clusters that are in Present State (kPresent)
vector <ara::ucm::SwClusterInfoType> SWClustersData;


void ara::ucm::storage::SWCLManager::AddSWCLChangeInfo(ara::ucm::SwClusterInfoType NewSWClusterInfo)
{
    ara::ucm::storage::SWCLManager::SWClustersData.push_back(NewSWClusterInfo);
}

void ara::ucm::storage::SWCLManager::CommitChanges()
{
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        ara::ucm::storage::ReversibleAction * Act = itr -> Action;
        Act -> CommitChanges();
    }
}

vector <ara::ucm::SwClusterInfoType> ara::ucm::storage::SWCLManager::GetPresentSWCLs()
{
    // Returns all the Clusters that are in Present State (kPresent)
    vector <ara::ucm::SwClusterInfoType> PresentSWClusters;
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if (itr->State == ara::ucm::SwClusterStateType::kPresent)
        {
            PresentSWClusters.push_back(*itr);
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
        if (itr->State != ara::ucm::SwClusterStateType::kPresent)
        {
            ChangeSWClusters.push_back(*itr);
        }
    }
    return ChangeSWClusters;
}

void ara::ucm::storage::SWCLManager::ResetSWCLChangeInfo()
{
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if (itr->State == ara::ucm::SwClusterStateType::kAdded)
        {
            SWClustersData.erase(itr);
        }
        else if ((itr->State == ara::ucm::SwClusterStateType::kRemoved) || (itr->State == ara::ucm::SwClusterStateType::kUpdated))
        {
            itr -> State = ara::ucm::SwClusterStateType::kPresent;
        }
    }
}

void ara::ucm::storage::SWCLManager::RevertChanges()
{
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        ara::ucm::storage::ReversibleAction * Act = itr -> Action;
        Act -> RevertChanges();
    }
}

void ara::ucm::storage::SWCLManager::SetSWCLState(ara::ucm::SwClusterInfoType ChangedSWCluster, ara::ucm::SwClusterStateType State)
{
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if ((itr->Name == ChangedSWCluster.Name) && (itr->Version == ChangedSWCluster.Version))
        {
            itr -> State = State;
        }
    }
}

void ara::ucm::storage::SWCLManager::RemoveSWCL(ara::ucm::SwClusterInfoType ChangedSWCluster)
{
    for(auto itr = SWClustersData.begin(); itr != SWClustersData.end(); ++itr)
    {
        if ((itr->Name == ChangedSWCluster.Name) && (itr->Version == ChangedSWCluster.Version))
        {
            SWClustersData.erase(itr);
        }
    }
}


