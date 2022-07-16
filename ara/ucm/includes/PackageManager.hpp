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
			class PackageManagerImpl : public ara::ucm::pkgmgr::PackageManagerSkeleton
			{
			public:
				PackageManagerImpl(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : PackageManagerSkeleton(I_id, mode)
				{}

				/* Transfer Operations */
				std::future<TransferStartOutput> TransferStart(uint64_t Size) override;

				std::future<TransferDataOutput> TransferData(TransferIdType id, ByteVectorType data, uint64_t blockCounter) override;

				std::future<TransferExitOutput> TransferExit(TransferIdType id) override;

				std::future<TransferDeleteOutput> TransferDelete(TransferIdType id) override;

				/* Process a previously transferred Software Package */
				std::future<ProcessSwPackageOutput> ProcessSwPackage(TransferIdType id) override;

				std::future<ActivateOutput> Activate() override;

				std::future<GetSwClusterInfoOutput> GetSwClusterInfo() override;

				std::future<RollbackOutput> Rollback() override;

				std::future<RevertProcessedSwPackagesOutput> RevertProcessedSwPackages() override;

				std::future<CurrentStatusField> GetCurrentStatus() override;

				std::future<FinishOutput> Finish() override;

			private:
				/* COMPOSITIONS */
				ara::ucm::transfer::SoftwarePackage SWPackageinstance;

				/* CURRENT STATUS IS REFERENCED BY THE PACKAGE MANAGER STATE CLASS */
				shared_ptr<ara::ucm::state::PackageManagerState> PackageManagerStateinstance = make_shared<ara::ucm::state::PackageManagerState>(ara::ucm::PackageManagerStatusType::kIdle);
			};
		}
	}
}
