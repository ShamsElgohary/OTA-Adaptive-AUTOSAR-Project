#ifndef TRANSFER_HPP_
#define TRANSFER_HPP_

namespace ara
{
	namespace ucm
	{
		namespace transfer
		{
			class TransferInstance
			{
				public:
					uint64_t expectedBytes;
					uint32_t BlockSize = 4;
					string path;
					uint64_t receivedBytes = 0;
					uint32_t receivedBlocks = 0;
					ara::ucm::SwPackageStateType TransferState;
					TransferIdType transferId;
			};
			class SoftwarePackage:public TransferInstance
			{
				private:
				public:

				static ara::ucm::TransferStartReturnType TransferStart(uint64_t Size);

				static ara::ucm::OperationResultType  TransferData(TransferIdType id, ByteVectorType data, uint64_t blockCounter);

				static ara::ucm::OperationResultType TransferExit(TransferIdType id);
			};

			class SynchronizedStorage
			{
				private:

				static vector <pair<ara::ucm::TransferIdType, ara::ucm::transfer::SoftwarePackage>> Data;
				public:

				static void AddItem(ara::ucm::TransferIdType, ara::ucm::transfer::SoftwarePackage);
				static void DeleteItem(ara::ucm::TransferIdType);
				static ara::ucm::transfer::SoftwarePackage * GetItem(ara::ucm::TransferIdType);
			};
		}
	}
}

#endif