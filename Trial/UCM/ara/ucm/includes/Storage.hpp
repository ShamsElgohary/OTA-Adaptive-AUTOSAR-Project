#pragma once


#include "UCM_Common.hpp"


namespace ara
{
	namespace ucm
	{
		namespace storage
		{
			class SWCLManager
			{
				private:
					static map <ara::ucm::storage::ReversibleAction * , ara::ucm::SwClusterInfoType> SWClustersData;

				public:
					static void AddSWCLChangeInfo(ara::ucm::SwClusterInfoType NewSWClusterInfo, ara::ucm::storage::ReversibleAction *);
					static void CommitChanges();
					static vector <ara::ucm::SwClusterInfoType> GetPresentSWCLs();
					static vector <ara::ucm::SwClusterInfoType> GetSWCLsChangeInfo();
					static void IndexActionsAndSWCLs();
					static void ResetSWCLChangeInfo();
					static void RevertChanges();
					static void SetSWCLState(ara::ucm::SwClusterInfoType ChangedSWCluster, ara::ucm::SwClusterStateType State);
					static void RemoveSWCL(ara::ucm::SwClusterInfoType ChangedSWCluster);
			};
			/*       PURE VIRUAL & HAS NO IMPLEMENTATION        */
			class ReversibleAction
			{
				private:
					

				public:
					ara::ucm::SwClusterInfoType SwClusterInfo;
					void CommitChanges();
					void Execute();
					void RevertChanges();
			};

			class InstallAction :public ReversibleAction
			{
				private:
				

				public:
					void CommitChanges();
					void Execute();
					void RevertChanges();
			};

			class RemoveAction :public ReversibleAction
			{
				private:
				

				public:
					void CommitChanges();
					void Execute();
					void RevertChanges();
			};

			class UpdateAction :public ReversibleAction
			{
				private:
				

				public:
					void CommitChanges();
					void Execute();
					void RevertChanges();
			};
		}
	}
}

