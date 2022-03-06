#include "../includes/Storage.hpp"

using namespace ara::ucm;


/*////////////////////////////////////////////////////////////////////////////

    SWCLManager Class Methods

////////////////////////////////////////////////////////////////////////////*/


map < shared_ptr<ara::ucm::storage::ReversibleAction > , ara::ucm::SwClusterInfoType> storage::SWCLManager::NewSWClusters;
vector<ara::ucm::SwClusterInfoType>  storage::SWCLManager::SWClusters;


shared_ptr<ara::ucm::storage::ReversibleAction > ara::ucm::storage::SWCLManager::AddSWCLChangeInfo(ara::ucm::SwClusterInfoType NewSWClusterInfo, ActionType ActType, std::string SWPath)
{
    shared_ptr<ara::ucm::storage::ReversibleAction > ReversibleAct;
    switch(ActType)
    {
        case ActionType::kInstall:
            ReversibleAct = make_shared<storage::InstallAction> (SWPath, NewSWClusterInfo);
            break;

        case ActionType::kRemove:

            for(auto itr = storage::SWCLManager::SWClusters.begin(); itr != storage::SWCLManager::SWClusters.end(); ++itr)
            {
                if (((itr->Name) == NewSWClusterInfo.Name) && ((itr->Version) == NewSWClusterInfo.Version))
                {
                    storage::SWCLManager::SWClusters.erase(itr);
                    break;
                }
            }

            ReversibleAct = make_shared<storage::RemoveAction> (SWPath, NewSWClusterInfo);
            break;

        case ActionType::kUpdate:

            StrongRevisionLabelString O_Version;
            for(auto itr = storage::SWCLManager::SWClusters.begin(); itr != storage::SWCLManager::SWClusters.end(); ++itr)
            {
                if ((itr->Name) == NewSWClusterInfo.Name)
                {
                    O_Version = itr->Version;
                    storage::SWCLManager::SWClusters.erase(itr);
                    break;
                }
            }

            ReversibleAct = make_shared<storage::UpdateAction> (SWPath, NewSWClusterInfo);
            ReversibleAct -> OldVersion = O_Version;
            break;
    }
    storage::SWCLManager::NewSWClusters.emplace(ReversibleAct, NewSWClusterInfo);
    ReversibleAct -> Execute();
    return ReversibleAct;
}

void ara::ucm::storage::SWCLManager::CommitChanges()
{
    for(auto itr = NewSWClusters.begin(); itr != NewSWClusters.end(); ++itr)
    {
        shared_ptr<ara::ucm::storage::ReversibleAction>  Act = itr -> first;
        Act -> CommitChanges();
    }

    auto itr = NewSWClusters.begin();
    while (itr != NewSWClusters.end())
    {
        /* REMOVE CLUSTERS THAT ARE IN KPRESENT OR KREMOVED FROM CHANGE LIST */
        if (((itr->second).State == ara::ucm::SwClusterStateType::kPresent) || 
            ((itr->second).State == ara::ucm::SwClusterStateType::kRemoved))
        {
            // `erase()` invalidates the iterator, use returned iterator
            itr = NewSWClusters.erase(itr);
        }
        else 
        {
            ++itr;
        }
    }
    return;
}

vector <ara::ucm::SwClusterInfoType> ara::ucm::storage::SWCLManager::GetPresentSWCLs()
{
    // Returns all the Clusters that are in Present State (kPresent)
    return SWCLManager::SWClusters;
}

vector <ara::ucm::SwClusterInfoType> ara::ucm::storage::SWCLManager::GetSWCLsChangeInfo()
{
    // Returns all the Clusters that has any changes (kAdded/kRemoved/kUpdated)
    vector <ara::ucm::SwClusterInfoType> ChangeSWClusters;
    for(auto itr = NewSWClusters.begin(); itr != NewSWClusters.end(); ++itr)
    {
        ChangeSWClusters.push_back(itr->second);
    }
    return ChangeSWClusters;
}

void ara::ucm::storage::SWCLManager::ResetSWCLChangeInfo()
{
    NewSWClusters.clear();
}

void ara::ucm::storage::SWCLManager::RevertChanges()
{
    for(auto itr = NewSWClusters.begin(); itr != NewSWClusters.end(); ++itr)
    {
        shared_ptr<ara::ucm::storage::ReversibleAction> Act = itr ->first;
        Act -> RevertChanges();
    }

    auto itr = NewSWClusters.begin();
    while (itr != NewSWClusters.end())
    {
        /* REMOVE CLUSTERS THAT ARE IN KPRESENT OR KREMOVED FROM CHANGE LIST */
        if (((itr->second).State == ara::ucm::SwClusterStateType::kPresent) || 
            ((itr->second).State == ara::ucm::SwClusterStateType::kRemoved))
        {
            // `erase()` invalidates the iterator, use returned iterator
            itr = NewSWClusters.erase(itr);
        }
        else 
        {
            ++itr;
        }
    }
    return;
}

void ara::ucm::storage::SWCLManager::SetSWCLState(ara::ucm::SwClusterInfoType ChangedSWCluster, ara::ucm::SwClusterStateType NewState)
{
    for(auto itr = NewSWClusters.begin(); itr != NewSWClusters.end(); ++itr)
    {
        if (((itr->second).Name == ChangedSWCluster.Name) && ((itr->second).Version == ChangedSWCluster.Version))
        {
            (itr->second).State = NewState;
            if(NewState == ara::ucm::SwClusterStateType::kPresent)
            {
                SWClusters.push_back(ChangedSWCluster);
            }
            break;
        }
    }
}

void ara::ucm::storage::SWCLManager::RemoveSWCL(ara::ucm::SwClusterInfoType ChangedSWCluster)
{
    for(auto itr = NewSWClusters.begin(); itr != NewSWClusters.end(); ++itr)
    {
        if ((((itr->second).Name) == ChangedSWCluster.Name) && (((itr->second).Version) == ChangedSWCluster.Version))
        {
            NewSWClusters.erase(itr);
            break;
        }
    }
}


void ara::ucm::storage::SWCLManager::PushInSWCLusters(ara::ucm::SwClusterInfoType ChangedSWCluster)
{
    storage::SWCLManager::SWClusters.push_back(ChangedSWCluster);
}


////////////////////////////////////////// END OF SWCLManager Class Methods ////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  



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

/* CONSTRUCTOR */////////////////////////////////////////
ara::ucm::storage::InstallAction::InstallAction(string swPackagePath, SwClusterInfoType SwClusterInfo): ReversibleAction(swPackagePath,SwClusterInfo)
{
    // DEFAULT CONSTRUCTOR
}


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

    /* ADD BIN TO THE  PATH IN THE FILE SYSTEM DIRECTORY */
    command = "mkdir " + clusterInFileSystem + "/" + "bin";
    system(command.c_str());

    /* ADD ETC TO THE  PATH IN THE FILE SYSTEM DIRECTORY */
    command = "mkdir " + clusterInFileSystem + "/" + "etc";
    system(command.c_str());

    /* MOVE THE PACKAGE CONTENTS TO THE FILE SYSTEM DIRECTORY */
    command = "find " + clusterTransferPath + " -executable -exec mv {} " + clusterInFileSystem + "/" + "bin " + "\\;";  
    system(command.c_str());

    /* MOVE THE PACKAGE CONTENTS TO THE FILE SYSTEM DIRECTORY */
    command = "mv "+ clusterTransferPath + ".json " + clusterInFileSystem + "/" + "etc";   
    system(command.c_str());

    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kAdded);

    /* REMOVE FOLDER FROM ZIP_PACKAGES FOLDER */
    command = "rm -r "+ this->swPackagePath;   
    system(command.c_str());
}

void ara::ucm::storage::InstallAction::CommitChanges()
{
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kPresent);
}

void ara::ucm::storage::InstallAction::RevertChanges()
{
    /* GET THE PATH TO THE FILE SYSTEM DIRECTORY ( VERSION NEEDED ) */
    string clusterInFileSystem { fileSystemPath + "/" + this->SwClusterInfo.Name }; 

    if (IsPathExist(clusterInFileSystem.c_str()))
    {
        /* REMOVE INSTALLED SW CLUSTER*/
        command = "rm -r " + clusterInFileSystem;
        system(command.c_str());
    }
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kRemoved);
}



/*////////////////////////////////////////////////////////////////////////////

    RemoveAction Class Methods

////////////////////////////////////////////////////////////////////////////*/

/* CONSTRUCTOR */
ara::ucm::storage::RemoveAction::RemoveAction(string swPackagePath, SwClusterInfoType SwClusterInfo): ReversibleAction(swPackagePath,SwClusterInfo)
{    
    // DEFAULT CONSTRUCTOR
}

void ara::ucm::storage::RemoveAction::Execute()
{
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kRemoved);


    /* REMOVE FOLDER FROM ZIP_PACKAGES FOLDER */
    command = "rm -r "+ this->swPackagePath;   
    system(command.c_str());
}

void ara::ucm::storage::RemoveAction::CommitChanges()
{
    /* GET THE PATH TO THE FILE SYSTEM DIRECTORY ( VERSION NEEDED ) */
    string clusterInFileSystem { fileSystemPath + "/" + this->SwClusterInfo.Name }; 

    if (IsPathExist(clusterInFileSystem.c_str()))
    {
        /* MOVE TO THE BACKUP FOLDER */
        command = "rm -r " + clusterInFileSystem;
        system(command.c_str());
    }
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kRemoved);
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
{
    // DEFAULT CONSTRUCTOR
}

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

    /* REMOVE FOLDER FROM ZIP_PACKAGES FOLDER */
    command = "rm -r "+ this->swPackagePath;   
    system(command.c_str());
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

    /* CREATE THE OLD VERSION OF THE SWCL INFO */
    ara::ucm::SwClusterInfoType OldSWCluster;
    OldSWCluster.Name = this->SwClusterInfo.Name;
    OldSWCluster.Version = this ->OldVersion;
    OldSWCluster.State = ara::ucm::SwClusterStateType::kPresent;

    /* REMOVE IT FROM EDITED SWCLUSTERS */
    ara::ucm::storage::SWCLManager::SetSWCLState(this->SwClusterInfo, ara::ucm::SwClusterStateType::kRemoved);

    /* ADD TO LIST OF PRESENT SWCLUSTERS */
    storage::SWCLManager::PushInSWCLusters(OldSWCluster);
}