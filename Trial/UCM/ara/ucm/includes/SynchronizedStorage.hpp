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

			static map <std::string, shared_ptr<ara::ucm::transfer::SoftwarePackage>>  Data;

			public:

			static void AddItem(ara::ucm::TransferIdType &transferId, shared_ptr<ara::ucm::transfer::SoftwarePackage> Package);
			static void DeleteItem(ara::ucm::TransferIdType &transferId);
			static shared_ptr<ara::ucm::transfer::SoftwarePackage> GetItem(ara::ucm::TransferIdType &transferId);
		};
	}
}

