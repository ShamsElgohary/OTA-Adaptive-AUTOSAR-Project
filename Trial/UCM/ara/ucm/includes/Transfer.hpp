#pragma once

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <utility>
#include <unistd.h>
#include <iomanip>
#include "nlohmann/json.hpp"
#include "UCM_Types.hpp"


namespace ara
{
	namespace ucm
	{
		namespace transfer
		{

			class TransferInstance
			{
				
				private:
					uint64_t expectedBytes;
					uint64_t receivedBytes = 0;
					string path;
					ara::ucm::SwPackageStateType TransferState;
					TransferIdType transferId;

				public:
					void SetExpectedBytes(uint64_t expectedBytes);
					void SetReceivedBytes(uint64_t receivedBytes);
					void SetTransferPath(string path);
					void SetTransferState(SwPackageStateType TransferState);
					void SetTransferId(TransferIdType transferId);

					uint64_t GetExpectedBytes();
					uint64_t GetReceivedBytes();
					string GetTransferPath();
					SwPackageStateType GetTransferState();
					TransferIdType &GetTransferId();			
				
			};



			class SoftwarePackage
			{
				private:

				TransferInstance TransferInfo;

				public:

				/* METHODS RELATED TO THE TRANSFER OPERATION OF THE UCM */
				static ara::ucm::TransferStartReturnType TransferStart(uint64_t Size);
				static ara::ucm::OperationResultType  TransferData(TransferIdType id, ByteVectorType data, uint64_t blockCounter);
				static ara::ucm::OperationResultType TransferExit(TransferIdType id);

				/* METHODS RELATED TO THE TransferInfo OBJECT SINCE DATA IS ENCAPSULATED*/
				void SetPackageExpectedBytes(uint64_t expectedBytes);
				void SetPackageReceivedBytes(uint64_t receivedBytes);
				void SetPackagePath(string path);
				void SetPackageState(SwPackageStateType State);
				void SetPackageId(TransferIdType Id);

				uint64_t GetPackageExpectedBytes();
				uint64_t GetPackageReceivedBytes();
				string GetPackagePath();
				SwPackageStateType GetPackageState();
				TransferIdType &GetPackageId();
				


			};

		}
	}
}

