#pragma once

#include "UCM_Common.hpp"
#include "State.hpp"
#include "Transfer.hpp"
#include "Parsing.hpp"
#include "Storage.hpp"
#include "SynchronizedStorage.hpp"
#include "PackageManagerSkeleton.hpp"

namespace ara
{
	namespace ucm
	{
		namespace pkgmgr
		{

			class PackageManager: public ara::com::skeleton::PackageManagerSkeleton
			{
			public:

			/* Transfer Operations */
			virtual std::future<TransferStartOutput> TransferStart(uint64_t Size) = 0;

			virtual std::future<OperationResultOutput> TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter) = 0;
			
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
			std::future<vector <ara::ucm::SwClusterInfoType>> GetPresentSWCLs();
			std::future<vector <ara::ucm::SwClusterInfoType>> GetSWCLsChangeInfo();

			/* Transfer Operations */
			std::future<TransferStartOutput> TransferStart(uint64_t Size);

			std::future<OperationResultOutput> TransferData(TransferIdType &id, ByteVectorType data, uint64_t blockCounter);
			
			std::future<OperationResultOutput> TransferExit(TransferIdType &id);

			std::future<OperationResultOutput> TransferDelete(TransferIdType &id);

			/* Process a previously transferred Software Package */
			std::future<OperationResultOutput> ProcessSwPackage(TransferIdType &id);

			std::future<OperationResultOutput> Activate();

			std::future<GetSWClusterInfoOutput> GetSwClusterInfo();

			std::future<OperationResultOutput> Rollback();

			std::future<OperationResultOutput> RevertProcessedSwPackages();

			std::future<GetCurrentStatusOutput> GetCurrentStatus();

			std::future<OperationResultOutput> Finish();

			PackageManagerImpl();

			~PackageManagerImpl();

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
