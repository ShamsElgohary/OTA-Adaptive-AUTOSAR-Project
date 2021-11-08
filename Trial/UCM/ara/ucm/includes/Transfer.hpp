#pragma once
#include "SynchronizedStorage.hpp"
#include "UCM_Types.hpp"
#include <stdlib.h>
#include <unistd.h>


namespace ara
{
	namespace ucm
	{
		namespace transfer
		{
			class SoftwarePackage: public TransferInstance
			{
				private:

				public:

				ara::ucm::TransferStartReturnType TransferStart(uint64_t Size);

				ara::ucm::OperationResultType  TransferData(TransferIdType id, ByteVectorType data, uint64_t blockCounter);

				ara::ucm::OperationResultType TransferExit(TransferIdType id);
			};

			class TransferInstance
			{
				public:
					uint64_t expectedBytes;
					string path;
					uint64_t receivedBytes = 0;
					ara::ucm::SwPackageStateType TransferState;
					TransferIdType transferId;
				
			};
		}
	}
}

