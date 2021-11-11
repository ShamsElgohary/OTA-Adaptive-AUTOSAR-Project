#include "includes/SynchronizedStorage.hpp"

using json = nlohmann::json;
using namespace ara::ucm;
using namespace ara::ucm::transfer;

map <std::string, ara::ucm::transfer::SoftwarePackage> ara::ucm::SynchronizedStorage::Data;

void SynchronizedStorage::AddItem(ara::ucm::TransferIdType &transferId, ara::ucm::transfer::SoftwarePackage Package)
{
    string StringID = to_string(transferId[0]);   
    for (uint8_t i =1 ; i < 16 ; i++)
    {
      StringID += '-' + to_string(transferId[i]);
    }
    Data.emplace(StringID, Package);
     // Data.insert(pair<TransferIdType, SoftwarePackage> (transferId, Package) );
}

void SynchronizedStorage::DeleteItem(TransferIdType &transferId)
{
    map<std::string, SoftwarePackage>::iterator itr;

    string StringID = to_string(transferId[0]);   
    for (uint8_t i =1 ; i < 16 ; i++)
    {
      StringID += '-' + to_string(transferId[i]);
    }
    /* IF THE CORRECT ID IS FOUND ERASE THE PACKAGE CORRESPONDING TO THIS ID*/   
    for (itr = Data.begin(); itr != Data.end(); ++itr) 
    {
        if ( itr->first == StringID)    
        {
            Data.erase(itr->first); 
            return;
        }
    }
}


ara::ucm::transfer::SoftwarePackage * SynchronizedStorage::GetItem(TransferIdType &transferId)
{
    map<std::string, SoftwarePackage>::iterator itr;

    string StringID = to_string(transferId[0]);   
    for (uint8_t i =1 ; i < 16 ; i++)
    {
      StringID += '-' + to_string(transferId[i]);
    }
    /* IF THE CORRECT ID IS FOUND RETURN THE PACKAGE CORRESPONDING TO THIS ID*/  
    for (itr = Data.begin(); itr != Data.end(); ++itr) 
    {
        if ( itr->first == StringID)    
        {
            return &(itr->second);
        }
    }
    /* NO MATCHING ID HAS BEEN FOUND */
   return nullptr; 
}

