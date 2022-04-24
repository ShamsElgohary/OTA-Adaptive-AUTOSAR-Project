#pragma once

#include "UCM_Common.hpp"

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
					uint32_t expectedBlocks;
					uint32_t receivedBlocks = 0;
					uint32_t blockSize = 512;
					string path;
					ara::ucm::SwPackageStateType TransferState;
					TransferIdType transferId;

				public:
					void SetExpectedBytes(uint64_t expectedBytes);
					void SetReceivedBytes(uint64_t receivedBytes);
					void SetExpectedBlocks(uint32_t expectedBlocks);
					void SetReceivedBlocks(uint32_t receivedBlocks);
					void SetBlockSize (uint32_t blockSize);
					void SetTransferPath(string path);
					void SetTransferState(SwPackageStateType TransferState);
					void SetTransferId(TransferIdType transferId);

					uint64_t GetExpectedBytes();
					uint64_t GetReceivedBytes();
					uint32_t GetExpectedBlocks();
					uint32_t GetReceivedBlocks();
					uint32_t GetBlockSize ();
					string GetTransferPath();
					SwPackageStateType GetTransferState();
					TransferIdType GetTransferId();			
				
			};



			class SoftwarePackage
			{
				private:

				TransferInstance TransferInfo;

				public:
				/* CONSTRUCTOR FOR TRANSFER INSTANCE */
				SoftwarePackage (uint64_t expectedBytes, string path, ara::ucm::SwPackageStateType TransferState, TransferIdType transferId);
				
				SoftwarePackage();

				/* METHODS RELATED TO THE TRANSFER OPERATION OF THE UCM */
				static ara::ucm::TransferStartReturnType TransferStartInternal(uint64_t Size);
				static ara::ucm::OperationResultType  TransferDataInternal(TransferIdType id, ByteVectorType data, uint64_t blockCounter);
				static ara::ucm::OperationResultType TransferExitInternal(TransferIdType id);
				static ara::ucm::OperationResultType TransferDeleteInternal(TransferIdType id);

				/* METHODS RELATED TO THE TransferInfo OBJECT SINCE DATA IS ENCAPSULATED */
				
				void SetPackageExpectedBytes(uint64_t expectedBytes);
				void SetPackageReceivedBytes(uint64_t receivedBytes);
				void SetPackageExpectedBlocks(uint32_t expectedBlocks);
				void SetPackageReceivedBlocks(uint32_t receivedBlocks);
				void SetPackageBlockSize (uint32_t blockSize);
				void SetPackagePath(string path);
				void SetPackageState(SwPackageStateType State);
				void SetPackageId(TransferIdType Id);

				uint64_t GetPackageExpectedBytes();
				uint64_t GetPackageReceivedBytes();
				uint32_t GetPackageExpectedBlocks();
				uint32_t GetPackageReceivedBlocks();
				uint32_t GetPackageBlockSize ();
				string GetPackagePath();
				SwPackageStateType GetPackageState();
				TransferIdType GetPackageId(); 
				
			};

		}
	}
}

