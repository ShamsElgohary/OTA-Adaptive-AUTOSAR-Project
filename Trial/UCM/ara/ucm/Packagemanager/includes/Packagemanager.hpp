#pragma once

#include "UCM_Types.hpp"
#include <stdlib.h>


namespace ara
{
	namespace ucm
	{
		namespace pkgmgr
		{
			class PackageManager {
			private:
				/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
				/* BLOCK SIZE */
				const uint32_t BlockSize = 512;

				/* Represents the state of UCM */
				inline static ara::ucm::PackageManagerStatusType UCMPackageManagerStatus {ara::ucm::PackageManagerStatusType::kIdle};
				/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

				/* Vector of Packages */
				inline static vector<SwPackageInfoType> CurrentSoftwarePackages {};

				/* Validate ID */
				ara::ucm::SwPackageInfoType* ara::ucm::pkgmgr::PackageManager::ValidateID(TransferIdType id);

			public:

				TransferStartReturnType StartTransfer(uint64_t Size);

				OperationResultType  TransferData(TransferIdType id, ByteVectorType data, uint64_t blockCounter);

				OperationResultType TransferExit(TransferIdType id);

			};
		}
	}
}

