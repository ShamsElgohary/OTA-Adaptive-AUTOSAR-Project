#pragma once


#include "UCM_Common.hpp"


namespace ara
{
	namespace ucm
	{
		namespace storage
		{

			/*       PURE VIRUAL HAS NO IMPLEMENTATION        */
			class ReversibleAction
			{
				/* PRIVATE TO ANYTHING EXCEPT DERIVED CLASSES */
				protected:
					string swPackagePath;
					ara::ucm::SwClusterInfoType SwClusterInfo;
					
					
				public:
					ReversibleAction(string swPackagePath, ara::ucm::SwClusterInfoType SwClusterInfo);
					StrongRevisionLabelString OldVersion = "";
					virtual void CommitChanges() = 0;
					virtual void Execute() = 0;
					virtual void RevertChanges() = 0;
			};

			class InstallAction :public ReversibleAction
			{
				private:
				

				public:
					explicit InstallAction(string swPackagePath, ara::ucm::SwClusterInfoType SwClusterInfo);
					void CommitChanges();
					void Execute();
					void RevertChanges();
			};

			class RemoveAction :public ReversibleAction
			{
				private:
				

				public:
					explicit RemoveAction(string swPackagePath, ara::ucm::SwClusterInfoType SwClusterInfo);
					void CommitChanges();
					void Execute();
					void RevertChanges();
			};

			class UpdateAction :public ReversibleAction
			{
				private:
				

				public:
					explicit UpdateAction(string swPackagePath, ara::ucm::SwClusterInfoType SwClusterInfo);
					void CommitChanges();
					void Execute();
					void RevertChanges();
			};

			class SWCLManager
			{
				private:
					static vector<ara::ucm::SwClusterInfoType> SWClusters;
					static map < shared_ptr<ara::ucm::storage::ReversibleAction > , ara::ucm::SwClusterInfoType> NewSWClusters;

				public:
					static shared_ptr<ara::ucm::storage::ReversibleAction > AddSWCLChangeInfo(ara::ucm::SwClusterInfoType NewSWClusterInfo, ActionType ActType, std::string SWPath);
					static void CommitChanges();
					static void RevertChanges();
					static vector <ara::ucm::SwClusterInfoType> GetPresentSWCLs();
					static vector <ara::ucm::SwClusterInfoType> GetSWCLsChangeInfo();
					static void IndexActionsAndSWCLs();
					static void ResetSWCLChangeInfo();
					static void PresentSWCLChangeInfo();
					static void SetSWCLState(ara::ucm::SwClusterInfoType ChangedSWCluster, ara::ucm::SwClusterStateType State);
					static void RemoveSWCL(ara::ucm::SwClusterInfoType ChangedSWCluster);
					static void PushInSWCLusters(ara::ucm::SwClusterInfoType ChangedSWCluster);
			};
		}
	}
}

