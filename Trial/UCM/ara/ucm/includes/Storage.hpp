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
					static map < shared_ptr<ara::ucm::storage::ReversibleAction > , ara::ucm::SwClusterInfoType> SWClustersData;

				public:
					static void AddSWCLChangeInfo(ara::ucm::SwClusterInfoType NewSWClusterInfo,shared_ptr<ara::ucm::storage::ReversibleAction> ReversibleAct);
					static void CommitChanges();
					static vector <ara::ucm::SwClusterInfoType> GetPresentSWCLs();
					static vector <ara::ucm::SwClusterInfoType> GetSWCLsChangeInfo();
					static void IndexActionsAndSWCLs();
					static void ResetSWCLChangeInfo();
					static void RevertChanges();
					static void SetSWCLState(ara::ucm::SwClusterInfoType ChangedSWCluster, ara::ucm::SwClusterStateType State);
					static void RemoveSWCL(ara::ucm::SwClusterInfoType ChangedSWCluster);
			};

		}
	}
}

