#pragma once

#include "Transfer.hpp"
#include "Parsing.hpp"
#include "Storage.hpp"
#include "UCM_Common.hpp"


namespace ara
{
	namespace ucm
	{
		namespace pkgmgr
		{
			class PackageManagerImpl {
			private:
			/* The current status of UCM */

			public:

			ara::ucm::TransferStartReturnType TransferStart(uint64_t Size);

			ara::ucm::OperationResultType  TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter);
			
			ara::ucm::OperationResultType TransferExit(TransferIdType &id);

			/* Process a previously transferred Software Package */
			void ProcessSwPackage(TransferIdType &id);





			};
		}
	}
}

