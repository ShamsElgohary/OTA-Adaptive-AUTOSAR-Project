#pragma once

#include "UCM_Common.hpp"
#include "Transfer.hpp"



namespace ara
{
	namespace ucm
	{
		class SynchronizedStorage
		{
			private:

			static map <std::string, ara::ucm::transfer::SoftwarePackage> Data;

			public:

			static void AddItem(ara::ucm::TransferIdType &transferId, ara::ucm::transfer::SoftwarePackage Package);
			static void DeleteItem(ara::ucm::TransferIdType &transferId);
			static ara::ucm::transfer::SoftwarePackage * GetItem(ara::ucm::TransferIdType &transferId);
		};
	}
}

