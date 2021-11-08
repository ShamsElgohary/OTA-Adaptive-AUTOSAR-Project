#include <iostream>
#include <fstream>
#include<iterator>
#include<vector>
#include "nlohmann/json.hpp"

#include "UCM_Types.hpp"
#include "Packagemanager.hpp"


using json = nlohmann::json;
using namespace ara::ucm::pkgmgr;
using namespace std;

ara::ucm::TransferStartReturnType ara::ucm::pkgmgr::PackageManager::TransferStart(uint64_t Size)
{
	;// Generate ID


	// Create JSON File


	// Appeand ll SW pKG


	// Ektbob fih l template


	// Emlo goz2 l Data Start fi l JSON

}


ara::ucm::OperationResultType ara::ucm::pkgmgr::PackageManager::TransferData(TransferIdType id, ByteVectorType data, uint64_t blockCounter)
{
	ara::ucm::OperationResultType ret = ara::ucm::OperationResultType::kSuccess;
	/* Check ID validity & Get SoftwarePackage  (CurrentSoftwarePackages > Vector of struct of SwPackageInfoType) */
	ara::ucm::SwPackageInfoType * SwPkg = ara::ucm::pkgmgr::PackageManager::ValidateID(id);
	if (SwPkg == nullptr)
	{
		cout << "[UCM TRANSFER DATA] InvalidTransferId" << endl;
		ret = ara::ucm::OperationResultType::kInvalidTransferId;
		return ret;
	}

	/* read a JSON file */
	/*********************************************************************/
	std::ifstream in("file.json");
	/*********************************************************************/
	json JSONObject;
	in >> JSONObject;

	/* Check for size of received block */
	if (data.size() > ara::ucm::pkgmgr::PackageManager::BlockSize)
	{
		cout << "[UCM TRANSFER DATA] IncorrectBlockSize" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectBlockSize;
		/* Write Output to JSON File */
		JSONObject.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
		return ret;
	}

	/* Validate Size with JSON File */
	uint64_t Size = JSONObject.at("UCM_OTA").at("TransferStart").at("Input").at("Size");
	if ((SwPkg -> ConsecutiveBytesReceived + data.size() > Size) || (Size == 0))
	{
		cout << "[UCM TRANSFER DATA] IncorrectSize" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectSize;
		/* Write Output to JSON File */
		JSONObject.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
		return ret;
	}

	/* Vaildate Block Counter */
	if (blockCounter != (SwPkg -> ConsecutiveBlocksReceived + 1))
	{
		cout << "[UCM TRANSFER DATA] IncorrectBlock" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectBlock;
		/* Write Output to JSON File */
		JSONObject.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
		return ret;
	}

	/* Write Input to JSON File (ID -> First Time) & (BlockCounter -> Increment) & (Data -> Append) */
	if (blockCounter == 0)
	{
		std::string Stringid (reinterpret_cast<char*>(id), sizeof(id));
		JSONObject.at("UCM_OTA").at("TransferData").at("Input").at("ID") = Stringid;
	}
	JSONObject.at("UCM_OTA").at("TransferData").at("Input").at("blockCounter") = blockCounter;
	std::string stringData (reinterpret_cast<const char *>(&data[0]), data.size());
	// std::string stringData (reinterpret_cast <char*>(data), sizeof(data));

	JSONObject.at("UCM_OTA").at("TransferData").at("Input").at("dataBlock") += stringData;

	/* Increment ConsecutiveBytesReceived & ConsecutiveBlocksReceived */
	SwPkg -> ConsecutiveBytesReceived += data.size();
	SwPkg -> ConsecutiveBlocksReceived += 1;

	/* Write Output to JSON File */
	JSONObject.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
	return ret;
}

ara::ucm::OperationResultType ara::ucm::pkgmgr::PackageManager::TransferExit(TransferIdType id)
{
	ara::ucm::OperationResultType ret = ara::ucm::OperationResultType::kSuccess;
	/* Check ID validity & Get SoftwarePackage  (CurrentSoftwarePackages > Vector of struct of SwPackageInfoType) */
	ara::ucm::SwPackageInfoType * SwPkg = ara::ucm::pkgmgr::PackageManager::ValidateID(id);
	if (SwPkg == nullptr)
	{
		cout << "[UCM TRANSFER EXIT] InvalidTransferId" << endl;
		ret = ara::ucm::OperationResultType::kInvalidTransferId;
		return ret;
	}

	/* read a JSON file */
	/*********************************************************************/
		std::ifstream in("file.json");
	/*********************************************************************/
	json JSONObject;
	in >> JSONObject;

	/* Write The ID in the TransferExit Input */
	std::string Stringid (reinterpret_cast<char*>(id), sizeof(id));
	JSONObject.at("UCM_OTA").at("TransferExit").at("Input").at("ID") = Stringid;


	/* Validate Size with JSON File */
	uint64_t Size = JSONObject.at("UCM_OTA").at("TransferStart").at("Input").at("Size");
	if (SwPkg -> ConsecutiveBytesReceived != Size)
	{
		cout << "[UCM TRANSFER EXIT] InsufficientData" << endl;
		ret = ara::ucm::OperationResultType::kInsufficientData;
		/* Write Output to JSON File */
		JSONObject.at("UCM_OTA").at("TransferExit").at("Output").at("err") = ret;
		return ret;
	}
	/* Validate Authentaction using Crypto */


	/* Validate Integrity using Crypto */


	/* Write Output to JSON File */
	JSONObject.at("UCM_OTA").at("TransferExit").at("Output").at("err") = ret;
	return ret;

}


ara::ucm::SwPackageInfoType* ara::ucm::pkgmgr::PackageManager::ValidateID(TransferIdType id)
{
	bool FoundID = false;
	bool Equalflag = true;
	vector<ara::ucm::SwPackageInfoType>::iterator SwPkg = ara::ucm::pkgmgr::PackageManager::CurrentSoftwarePackages.begin();
	for(SwPkg; SwPkg < ara::ucm::pkgmgr::PackageManager::CurrentSoftwarePackages.end(); SwPkg++)
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			if (SwPkg->TransferID[i] == id[i])
			{
				Equalflag = 1;
			}
			else
			{
				Equalflag = 0;
				break;
			}
		}
		if (Equalflag == 1)
		{
			ara::ucm::SwPackageInfoType * SwPkgPtr =&(*SwPkg);
			return SwPkgPtr;
		}
	}
	if (FoundID == false)
	{
		cout << "[UCM TRANSFER DATA] InvalidTransferId" << endl;
		return nullptr;
	}
}
