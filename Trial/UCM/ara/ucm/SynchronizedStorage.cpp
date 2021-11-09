#include "includes/SynchronizedStorage.hpp"

using json = nlohmann::json;
using namespace ara::ucm;
using namespace ara::ucm::transfer;


void SynchronizedStorage::AddItem(TransferIdType transferId, SoftwarePackage Package)
{
    Data.emplace(transferId, Package);
     // Data.insert(pair<TransferIdType, SoftwarePackage> (transferId, Package) );
}

void SynchronizedStorage::DeleteItem(TransferIdType transferId)
{
    map<TransferIdType, SoftwarePackage>::iterator itr;
    bool CorrectID;
    /* IF THE CORRECT ID IS FOUND ERASE THE PACKAGE CORRESPONDING TO THIS ID*/   
    for (itr = Data.begin(); itr != Data.end(); ++itr) {
        CorrectID = true;

        for (uint8_t i = 0; i < 16 ; i++)
        {          
            if ( itr->first[i] != transferId[i])    
            {
                CorrectID = false;
            }
        }

        if (CorrectID == true)
        {
            Data.erase(itr->first); 
            break;        
        }
    }
}


SoftwarePackage *SynchronizedStorage::GetItem(TransferIdType transferId)
{
    map<TransferIdType, SoftwarePackage>::iterator itr;
    bool CorrectID;
    /* IF THE CORRECT ID IS FOUND RETURN THE PACKAGE CORRESPONDING TO THIS ID*/  
    for (itr = Data.begin(); itr != Data.end(); ++itr) {
        CorrectID = true;

        for (uint8_t i = 0; i < 16 ; i++)
        {          
            if ( itr->first[i] != transferId[i])     /* CHECK IF ID DOESN'T MATCH */
            {
                CorrectID = false;
            }
        }

        if (CorrectID == true)
        {
            return &(itr->second);           
        }
    }

    /* NO MATCHING ID HAS BEEN FOUND */
   return nullptr; 
}

