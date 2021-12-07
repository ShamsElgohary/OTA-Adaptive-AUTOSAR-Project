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

			class PackageManager
			{
			public:

			/* Transfer Operations */
			virtual ara::ucm::TransferStartReturnType TransferStart(uint64_t Size) = 0;

			virtual ara::ucm::OperationResultType  TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter) = 0;
			
			virtual ara::ucm::OperationResultType TransferExit(TransferIdType &id) = 0;

			virtual ara::ucm::OperationResultType TransferDelete(TransferIdType &id) = 0;

			/* Process a previously transferred Software Package */
			virtual ara::ucm::OperationResultType ProcessSwPackage(TransferIdType &id) = 0;

			virtual ara::ucm::OperationResultType Activate() = 0;

			virtual vector<ara::ucm::SwClusterInfoType> GetSwClusterInfo() = 0;

			virtual ara::ucm::OperationResultType Rollback() = 0;

			virtual ara::ucm::OperationResultType RevertProcessedSwPackages() = 0;

			virtual PackageManagerStatusType GetCurrentStatus() = 0;

			private:


			};
			

			class PackageManagerImpl : public PackageManager 
			{
			public:

			/* Transfer Operations */
			ara::ucm::TransferStartReturnType TransferStart(uint64_t Size);

			ara::ucm::OperationResultType  TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter);
			
			ara::ucm::OperationResultType TransferExit(TransferIdType &id);

			ara::ucm::OperationResultType TransferDelete(TransferIdType &id);

			/* Process a previously transferred Software Package */
			ara::ucm::OperationResultType ProcessSwPackage(TransferIdType &id);

			ara::ucm::OperationResultType Activate();

			vector<ara::ucm::SwClusterInfoType> GetSwClusterInfo();

			ara::ucm::OperationResultType Rollback();

			ara::ucm::OperationResultType RevertProcessedSwPackages();

			PackageManagerStatusType GetCurrentStatus();

			private:
			
			/* The current status of UCM */
			static PackageManagerStatusType CurrentStatus;
			/* COMPOSITIONS */
			
			ara::ucm::transfer::SoftwarePackage SWPackageinstance;

			/* CURRENT STATUS IS REFERENCED BY THE PACKAGE MANAGER STATE CLASS */
			shared_ptr<ara::ucm::state::PackageManagerState> PackageManagerStateinstance \
			= make_shared<ara::ucm::state::PackageManagerState>(CurrentStatus);

			};
		}
	}
}
