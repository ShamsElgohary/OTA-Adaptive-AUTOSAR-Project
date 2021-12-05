#pragma once

#include "Transfer.hpp"
#include "Parsing.hpp"
#include "Storage.hpp"
#include "SynchronizedStorage.hpp"
#include "UCM_Common.hpp"
#include "State.hpp"


namespace ara
{
	namespace ucm
	{
		namespace pkgmgr
		{
			class PackageManagerImpl {
			private:
			/* The current status of UCM */
			static PackageManagerStatusType CurrentStatus;

			/* COMPOSITIONS */
			
			ara::ucm::transfer::SoftwarePackage SWPackageinstance;

			/* CURRENT STATUS IS REFERENCED BY THE PACKAGE MANAGER STATE CLASS */
			shared_ptr<ara::ucm::state::PackageManagerState> PackageManagerStateinstance \
			= make_shared<ara::ucm::state::PackageManagerState>(CurrentStatus);

			public:

			/* Transfer Operations */
			ara::ucm::TransferStartReturnType TransferStart(uint64_t Size);

			ara::ucm::OperationResultType  TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter);
			
			ara::ucm::OperationResultType TransferExit(TransferIdType &id);

			ara::ucm::OperationResultType TransferDelete(TransferIdType &id);

			/* Process a previously transferred Software Package */
			ara::ucm::OperationResultType ProcessSwPackage(TransferIdType &id);



			};
		}
	}
}

