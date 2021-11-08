#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <utility>
#include <unistd.h>
#include <iomanip>

#include <nlohmann/json.hpp>

#include "UCM_Types.hpp"
#include "Transfer.hpp"


using json = nlohmann::json;
using namespace ara::ucm::transfer;
using namespace ara::ucm;
using namespace std;



ara::ucm::TransferStartReturnType ara::ucm::transfer::SoftwarePackage::TransferStart(uint64_t Size)
{
    /* This  is the file where ota will read from */
    string jsonPath = "UCM_PackageManagerPipe.json";

	/* Information of the NewPackage to be transfered */
	SoftwarePackage NewPackageInfo;

	/* This will hold the data that will be returned from this operation */
	TransferStartReturnType StartTransferOutput;

	/* Number Of Blocks, Size in bytes and each 4 bytes are 1 block */
	StartTransferOutput.BlockSize = ceil(Size / NewPackageInfo.BlockSize);

    /* Initialize random seed: */
	srand (time(NULL));

	/* Generate a Transfer ID  */
    for (uint8_t i =0 ; i < 16 ; i++)
    {
    	  NewPackageInfo.transferId[i] = rand() % UINT8_MAX ;
          StartTransferOutput.id[i] = NewPackageInfo.transferId[i];
    }

	/* So OTA Can Read ID */
	string StringID = to_string(NewPackageInfo.transferId[0]);   
    for (uint8_t i =1 ; i < 16 ; i++)
    {
      StringID += '-' + to_string(NewPackageInfo.transferId[i] );
    }

    /* Initiliaze receive bytes to zero (None yet) */
    NewPackageInfo.receivedBytes = 0;

    /* Get path of packages folder */
    NewPackageInfo.path = GetCurrentDirectory()+ "//Packages";
    /* change dir to path of packages folder */
    chdir(NewPackageInfo.path.c_str());

    /* Create Zipped File */
    ofstream MyFile("TransferID:" + StringID + ".zip");
    //MyFile.write(&StringID[0],StringID.size());
    MyFile.close();

    ara::ucm::transfer::SynchronizedStorage::AddItem(NewPackageInfo.transferId, NewPackageInfo);

    /* FIRST MAKING THE JSON FILE MODIFIABLE */
    nlohmann::json UCM_PipeData;

    ifstream JSON_Modifiable(jsonPath);
    JSON_Modifiable >> UCM_PipeData;

    /* WRITING OUR OPERATION INPUT */
    UCM_PipeData.at("UCM_OTA").at("TransferStart").at("Input").at("Size") = Size;
    /* WRITING OUR OPERATION OUTPUT */
    UCM_PipeData.at("UCM_OTA").at("TransferStart").at("Output").at("ID") =  StringID;

    /* WRITING THE ADJUSTMENTS TO THE FILE */
    ofstream JSON_PipeFile( NewPackageInfo.path );
	JSON_PipeFile<<setw(4)<< UCM_PipeData <<endl;

    StartTransferOutput.TransferStartResult = TransferStartSuccessType::kSuccess;
    NewPackageInfo.TransferState = SwPackageStateType::kTransferring;
    
	return StartTransferOutput;
}

ara::ucm::OperationResultType  ara::ucm::transfer::SoftwarePackage::TransferData (ara::ucm::TransferIdType id, ara::ucm::ByteVectorType data, uint64_t blockCounter)
{
    /****************************************** ERROR CHECKING ******************************************/
    
    ara::ucm::OperationResultType ret = ara::ucm::OperationResultType::kSuccess;
	/* Check ID validity & Get SoftwarePackage  (CurrentSoftwarePackages > Vector of struct of SwPackageInfoType) */
	ara::ucm::transfer::SoftwarePackage * SwPkg = ara::ucm::transfer::SynchronizedStorage::GetItem(id);
	if (SwPkg == nullptr)
	{
		cout << "[UCM TRANSFER DATA] InvalidTransferId" << endl;
		ret = ara::ucm::OperationResultType::kInvalidTransferId;
		return ret;
	}

    /* Change The ID to String To get paths */
    string StringID = to_string(SwPkg -> transferId[0]);   
    for (uint8_t i =1 ; i < 16 ; i++)
    {
      StringID += '-' + to_string(SwPkg -> transferId[i]);
    }

	/* read a JSON file */
	std::ifstream JSONIn(GetCurrentDirectory()+ "//Packages//JSONs//"+"TransferID:" + StringID + ".JSON");
    std::ofstream JSONOut(GetCurrentDirectory()+ "//Packages//JSONs//"+"TransferID:" + StringID + ".JSON");
	json JSONObject;
	JSONIn >> JSONObject;

	/* Check for size of received block */
	if (data.size() > (SwPkg -> BlockSize))
	{
		cout << "[UCM TRANSFER DATA] IncorrectBlockSize" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectBlockSize;
		/* Write Output to JSON File */
		JSONObject.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
        JSONOut << JSONObject;
		return ret;
	}

	/* Validate Size with Transfer Instance */
	uint64_t Size = SwPkg->expectedBytes;
	if ((SwPkg -> receivedBytes + data.size() > Size) || (Size == 0))
	{
		cout << "[UCM TRANSFER DATA] IncorrectSize" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectSize;
		/* Write Output to JSON File */
		JSONObject.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
        JSONOut << JSONObject;
		return ret;
	}

	/* Vaildate Block Counter */
	if (blockCounter != (SwPkg -> receivedBlocks))
	{
		cout << "[UCM TRANSFER DATA] IncorrectBlock" << endl;
		ret = ara::ucm::OperationResultType::kIncorrectBlock;
		/* Write Output to JSON File */
		JSONObject.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
        JSONOut << JSONObject;
		return ret;
	}

    /****************************************** UPDATE ZIP FILE ******************************************/
    /* Constract the Path */
    string zipPath = SwPkg -> path + "TransferID:" + StringID + ".zip";

    /* Open File To Update */
    std::ofstream zipIn(zipPath, ios::app);

    /* Convert Data to String */
    std::string stringData (reinterpret_cast<const char *>(&data[0]), data.size());

    /* Append Data to zip File */
    zipIn << stringData;

    /* Close File */
    zipIn.close();

    /****************************************** WRITE INPUT & OUTPUT DATA TO JSON FILE ******************************************/
	/* Write Input to JSON File (ID -> First Time) & (BlockCounter -> Increment) & (Data -> Append) */
	if (blockCounter == 0)
	{
		std::string Stringid (reinterpret_cast<char*>(id), sizeof(id));
		JSONObject.at("UCM_OTA").at("TransferData").at("Input").at("ID") = Stringid;
	}
	JSONObject.at("UCM_OTA").at("TransferData").at("Input").at("blockCounter") = blockCounter;
	
	// std::string stringData (reinterpret_cast <char*>(data), sizeof(data));
	JSONObject.at("UCM_OTA").at("TransferData").at("Input").at("dataBlock") += stringData;

	/* Increment ConsecutiveBytesReceived & ConsecutiveBlocksReceived */
	SwPkg -> receivedBytes += data.size();
	SwPkg -> receivedBlocks += 1;

	/* Write Output to JSON File */
	JSONObject.at("UCM_OTA").at("TransferData").at("Output").at("err") = ret;
    JSONOut << JSONObject;
	return ret;
}


ara::ucm::OperationResultType ara::ucm::transfer::SoftwarePackage::TransferExit(ara::ucm::TransferIdType id)
{

}



void ara::ucm::transfer::SynchronizedStorage::AddItem(ara::ucm::TransferIdType id, ara::ucm::transfer::SoftwarePackage SwPkg)
{
    pair<ara::ucm::TransferIdType,ara::ucm::transfer::SoftwarePackage > SWData;
	// SWData.first = id;
	for (uint8_t j = 0; j < 16 ; j++)
	{
		(*((SWData.first)+j)) = id[j];
	}
	SWData.second = SwPkg;
	ara::ucm::transfer::SynchronizedStorage::Data.push_back(SWData);
}

void ara::ucm::transfer::SynchronizedStorage::DeleteItem(ara::ucm::TransferIdType id)
{
	auto i = ara::ucm::transfer::SynchronizedStorage::Data.begin();
	bool Equalflag = true;
	for(auto Item = Data.begin(); i != Data.end() ; i++)
	{
		for (uint8_t j = 0; j < 16 ; j++)
		{
			if (id[j] != (*(((*Item).first)+j)))
			{
				Equalflag = false;
				break;
			}
		}
		if (Equalflag == true)
		{
			Data.erase(Item);
		}
	}
}

ara::ucm::transfer::SoftwarePackage * ara::ucm::transfer::SynchronizedStorage::GetItem(ara::ucm::TransferIdType id)
{
	auto i = ara::ucm::transfer::SynchronizedStorage::Data.begin();
	bool Equalflag = true;
	for(auto Item = Data.begin(); i != Data.end() ; i++)
	{
		for (uint8_t j = 0; j < 16 ; j++)
		{
			if (id[j] != (*(((*Item).first)+j)))
			{
				Equalflag = false;
				break;
			}
		}
		if (Equalflag == true)
		{
			ara::ucm::transfer::SoftwarePackage * PKGData = &((*Item).second);
			return PKGData;
		}
	}
	return nullptr;
}


/* Used to get the path of our current directory */
string GetCurrentDirectory()
{
    char directoryPath[FILENAME_MAX];
    getcwd(directoryPath,FILENAME_MAX);
    return directoryPath;
}


