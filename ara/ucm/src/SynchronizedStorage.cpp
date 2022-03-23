#include "../includes/SynchronizedStorage.hpp"

using json = nlohmann::json;
using namespace ara::ucm;
using namespace ara::ucm::transfer;

map<std::string, shared_ptr<ara::ucm::transfer::SoftwarePackage>> ara::ucm::SynchronizedStorage::Data;

void SynchronizedStorage::AddItem(ara::ucm::TransferIdType &transferId, shared_ptr<ara::ucm::transfer::SoftwarePackage> Package)
{
    string StringID{Convert2StringID(transferId)};
    Data.emplace(StringID, Package);
    // Data.insert(pair<TransferIdType, SoftwarePackage> (transferId, Package) );
}

void SynchronizedStorage::DeleteItem(TransferIdType &transferId)
{
    string StringID{Convert2StringID(transferId)};

    map<std::string, shared_ptr<SoftwarePackage>>::iterator itr;
    /* IF THE CORRECT ID IS FOUND ERASE THE PACKAGE CORRESPONDING TO THIS ID*/
    for (itr = Data.begin(); itr != Data.end(); ++itr)
    {
        if (itr->first == StringID)
        {
            Data.erase(itr);
            return;
        }
    }
}

shared_ptr<ara::ucm::transfer::SoftwarePackage> SynchronizedStorage::GetItem(TransferIdType &transferId)
{
    string StringID{Convert2StringID(transferId)};
    map<std::string, shared_ptr<SoftwarePackage>>::iterator itr;
    /* IF THE CORRECT ID IS FOUND RETURN THE PACKAGE CORRESPONDING TO THIS ID*/
    for (itr = Data.begin(); itr != Data.end(); ++itr)
    {
        if (itr->first == StringID)
        {
            return (itr->second);
        }
    }
    /* NO MATCHING ID HAS BEEN FOUND */
    return nullptr;
}
