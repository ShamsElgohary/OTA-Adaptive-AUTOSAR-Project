#pragma once

#include <stdlib.h>
#include <vector>
#include <iterator>

#include "UCM_Types.hpp"



namespace ara
{
	namespace ucm
	{
		namespace storage
		{
			class SWCLManager
			{
				private:
					static vector <ara::ucm::SwClusterInfoType> SWClustersData;

				public:
					static void AddSWCLChangeInfo(ara::ucm::SwClusterInfoType);
					static void CommitChanges();
					static ara::ucm::SwClusterInfoType GetPresentSWCLs();
					static ara::ucm::SwClusterInfoType GetSWCLsChangeInfo();
					static void IndexActionsAndSWCLs();
					static void ResetSWCLChangeInfo();
					static void RevertChanges();
			};
			/*       PURE VIRUAL & HAS NO IMPLEMENTATION        */
			class ReversibleAction
			{
				private:
				

				public:
					virtual void CommitChanges() = 0;
					virtual ara::ucm::OperationResultType Execute() = 0;
					virtual void RevertChanges() = 0;
			};

			class InstallAction :public ReversibleAction
			{
				private:
				

				public:
					void CommitChanges();
					ara::ucm::OperationResultType Execute();
					void RevertChanges();
			};

			class RemoveAction :public ReversibleAction
			{
				private:
				

				public:
					void CommitChanges();
					ara::ucm::OperationResultType Execute();
					void RevertChanges();
			};

			class UpdateAction :public ReversibleAction
			{
				private:
				

				public:
					void CommitChanges();
					ara::ucm::OperationResultType Execute();
					void RevertChanges();
			};


		}
	}
}

