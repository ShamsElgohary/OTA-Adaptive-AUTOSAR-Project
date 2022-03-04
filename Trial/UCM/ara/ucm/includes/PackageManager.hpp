#pragma once

#include "Transfer.hpp"
#include "Parsing.hpp"
#include "Storage.hpp"
#include "SynchronizedStorage.hpp"
#include "UCM_Common.hpp"
#include "State.hpp"

#include "PackageManagerSkeleton.hpp"

namespace ara
{
	namespace ucm
	{
		namespace pkgmgr
		{

			class PackageManager : public ara::com::skeleton::PackageManagerSkeleton
			{
			public:

			/* Transfer Operations */
			virtual std::future<TransferStartOutput> TransferStart(uint64_t Size) = 0;

			virtual ara::ucm::OperationResultType  TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter) = 0;
			
			virtual std::future<OperationResultOutput> TransferExit(TransferIdType &id) = 0;

			virtual std::future<OperationResultOutput> TransferDelete(TransferIdType &id) = 0;

			/* Process a previously transferred Software Package */
			virtual std::future<OperationResultOutput> ProcessSwPackage(TransferIdType &id) = 0;

			virtual std::future<OperationResultOutput> Activate() = 0;

			virtual std::future<GetSWClusterInfoOutput> GetSwClusterInfo() = 0;

			virtual std::future<OperationResultOutput> Rollback() = 0;

			virtual std::future<OperationResultOutput> RevertProcessedSwPackages() = 0;

			virtual std::future<GetCurrentStatusOutput> GetCurrentStatus() = 0;

			virtual std::future<OperationResultOutput> Finish() = 0;


			private:


			};
			

			class PackageManagerImpl : public PackageManager 
			{
			public:

			/* Transfer Operations */
			virtual std::future<TransferStartOutput> TransferStart(uint64_t Size);

			virtual ara::ucm::OperationResultType  TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter);
			
			virtual std::future<OperationResultOutput> TransferExit(TransferIdType &id);

			virtual std::future<OperationResultOutput> TransferDelete(TransferIdType &id);

			/* Process a previously transferred Software Package */
			virtual std::future<OperationResultOutput> ProcessSwPackage(TransferIdType &id);

			virtual std::future<OperationResultOutput> Activate();

			virtual std::future<GetSWClusterInfoOutput> GetSwClusterInfo();

			virtual std::future<OperationResultOutput> Rollback();

			virtual std::future<OperationResultOutput> RevertProcessedSwPackages();

			virtual std::future<GetCurrentStatusOutput> GetCurrentStatus();

			virtual std::future<OperationResultOutput> Finish();

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
