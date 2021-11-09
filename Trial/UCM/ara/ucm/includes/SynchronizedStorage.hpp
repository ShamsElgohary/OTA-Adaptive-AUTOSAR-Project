#pragma once

#include <stdlib.h>
#include <map>
#include <iterator>

#include "UCM_Types.hpp"
#include "Transfer.hpp"



namespace ara
{
	namespace ucm
	{
		class SynchronizedStorage
		{
			private:

			static map <ara::ucm::TransferIdType, ara::ucm::transfer::SoftwarePackage> Data;

			public:

			static void AddItem(ara::ucm::TransferIdType transferId, ara::ucm::transfer::SoftwarePackage Package);
			static void DeleteItem(ara::ucm::TransferIdType transferId);
			static ara::ucm::transfer::SoftwarePackage * GetItem(ara::ucm::TransferIdType transferId);
		};
	}
}

