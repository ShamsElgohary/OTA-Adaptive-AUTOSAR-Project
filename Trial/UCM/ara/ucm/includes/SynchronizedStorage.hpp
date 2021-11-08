#pragma once
#include <stdlib.h>
#include <map>
#include "UCM_Types.hpp"
#include "Transfer.hpp"



namespace ara
{
	namespace ucm
	{
		class SynchronizedStorage
		{
			private:

			static map <uint32_t, pair<ara::ucm::TransferIdType, ara::ucm::transfer::SoftwarePackage>> Data;

			public:

			static void AddItem(ara::ucm::TransferIdType, ara::ucm::transfer::SoftwarePackage);
			static void DeleteItem(ara::ucm::TransferIdType);
			static ara::ucm::transfer::SoftwarePackage GetItem(ara::ucm::TransferIdType);
		};
	}
}

