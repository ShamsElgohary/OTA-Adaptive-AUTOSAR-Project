/*****************************************************************************/
/* Functional Cluster : UCM
/*
/* File Name: UCM_Types.h
/*
/* Description: Header file that lists all types provided by the UCM.
/*
/******************************************************************************/

#pragma once
#include <iostream>
#include <vector>

using namespace std;

namespace ara
{
	namespace ucm
	{

		/* UCM Module Instantiation Identifier */
		using UCMIdentifierType = string;

		/* Represents a handle identifier used to reference a particular transfer request */
		using TransferIdType = uint8_t[16];
		//using TransferIdType = uint64_t;		// For Simplicity Right now

		/* SW Cluster or SW Package shortName attribute inherited from referrable metaClass */
		using SwNameType = string;

		/* Represents a dynamic size array of Software Cluster names */
		using  SwNameVectorType = vector<SwNameType>;

		/* Primitive type representing SoftwareCluster (SoftwarePackage) version */
		using StrongRevisionLabelString = string;

		/* Represents the information of a Software Package (Software Cluster) name and version */
		typedef struct
		{
			SwNameType Name;
			StrongRevisionLabelString Version;

		}SwNameVersionType;

		/* Represents a dynamic size array of Software Name and Version */
		using SwNameVersionVectorType = vector<SwNameVersionType>;

		/* Byte vector representing raw data */
		using ByteVectorType = vector<uint8_t>;

		/* Represents the state of a Software Package on the Platform  */
		enum class SwPackageStateType : uint8_t
		{
			kTransferring = 0U,
			kTransferred = 1U,
			kProcessing = 2U,
			kProcessed = 3U,
			kProcessingStream = 4U

		};


		/* Represents the information of a Software Package */
		typedef struct
		{
			SwNameType Name;
			StrongRevisionLabelString Version;
			TransferIdType TransferID;

			// El Wesli
			uint64_t ConsecutiveBytesReceived;

			// El Wesli
			uint64_t ConsecutiveBlocksReceived;
			SwPackageStateType State;
			
			/* Added on Standerd to represent the path to the JSON File */
			string JsonPath;

		}SwPackageInfoType;


		/* Represents a dynamic size array of Software Packages */
		typedef vector<SwPackageInfoType> SwPackageInfoVectorType;


		/* Contains general information related to SoftwareCluster that can be used by Vehicle
		* Driver Application or Human Interface. */
		typedef struct
		{
			SwNameType Name;
			StrongRevisionLabelString Version;
			string TypeApproval;
			string License;
			string ReleaseNotes;
			uint64_t Size;

		}SwDescType;

		/* Represents a dynamic size array of SoftwareCluster description */
		typedef vector<SwDescType> SwDescVectorType;

		/* Represents the state of a SoftwareCluster on the adaptive platform.  */
		enum class SwClusterStateType : uint8_t
		{
			kPresent = 0U,
			kAdded = 1U,
			kUpdated = 2U,
			kRemoved = 3U,

		};

		/* Represents the information of a SoftwareCluster */
		typedef struct
		{
			SwNameType Name;
			StrongRevisionLabelString Version;
			SwClusterStateType State;
			ara::ucm::storage::ReversibleAction * Action;

		}SwClusterInfoType;

		/* Represents a dynamic size array of SoftwareClusters */
		typedef vector<SwClusterInfoType> SwClusterInfoVectorType;

		/* Represents the state of UCM */
		enum class PackageManagerStatusType : uint8_t
		{
			kIdle = 0U,
			kReady = 1U,
			kBusy = 2U,
			kActivating = 3U,
			kActivated = 4U,
			kRollingBack = 5U,
			kRolledBack = 6U,
			kCleaningUp = 7U,
			kVerifying = 8U,

		};


		/* Represents the UCM action */
		enum class ActionType : uint8_t
		{
			kUpdate = 0U,
			kInstall = 1U,
			kRemove = 2U,

		};


		/* Represents the result of UCM action */
		enum class ResultType : uint8_t
		{
			kSuccessfull = 0U,
			kFailed = 1U,

		};

		/* Time refers to the activation time of the software cluster. It is represented in milliseconds
		* of UCMs action resolution since 01.01.1970 (UTC).  */
		typedef struct
		{
			uint64_t Time;
			SwNameType Name;
			StrongRevisionLabelString Version;
			ActionType Action;
			ResultType Resolution;

		}GetHistoryType;

		/* Represents a list of UCM actions */
		typedef vector<GetHistoryType> GetHistoryVectorType;


		/* Campaign failure type */
		enum class CampaignFailureType : uint8_t
		{
			kUCMError = 1U,
			kInvalidVehiclePackageManifest = 2U,
			kSubordinateNotAvailableOnTheNetwork = 3U,
			kVehicleStateManagerCommunicationError = 4U,
			kVehicleDriverInterfaceCommunicationError = 5U,
			kCampaignCancelled = 6U,

		};



		/* UCM Software Package step at which error occurred */
		enum class SoftwarePackageStepType : uint8_t
		{
			kTransfer = 0U,
			kProcess = 1U,
			kPreActivate = 2U,
			kVerify = 3U,

		};

		/* UCM ERROR */
		typedef struct
		{
			UCMIdentifierType id;
			SoftwarePackageStepType SoftwarePackageStep;
			uint8_t ReturnedError;

		}UCMStepErrorType;

		typedef struct
		{
			CampaignFailureType CampaignFailure;
			UCMStepErrorType UCMStepError;
		}CampaignErrorType;


		/* History of an UCM */
		typedef struct
		{
			UCMIdentifierType id;
			GetHistoryVectorType HistoryVector;

		}HistoryVectorType;

		/* Campaign history */
		typedef struct
		{
			CampaignErrorType CampaignError;
			HistoryVectorType HistoryVector;

		} CampaignHistoryType;

		/* Represents the status of Campaign */
		enum class CampaignStateType : uint8_t
		{
			kIdle = 0U,
			kSyncing = 1U,
			kVehiclePackageTransferring = 2U,
			kSoftwarePackage_Transferring = 3U,
			kProcessing = 4U,
			kActivating = 5U,
			kVehicleChecking = 6U

		};

		/* Represents the state of an update from OTA Client perspective.Represents the state of an update from OTA Client perspective */
		enum class TransferStateType : uint8_t
		{
			kIdle = 0U,
			kTransferring = 1U,
			kUpdating = 2U,
			kCancelling = 3U,

		};


		/* The type of the Safety Policy */
		typedef string SafetyPolicyType;


		/////////////////////////////////// IMPLEMENTATION SPECIFIC TYPES //////////////////////////////////////


		// Shayef nseb da w nktb kol el errors feh w nstkhdmo fe ay operation badl mn3od nkrar el code le kol function lesa el functions kter

		/// &ad yezbat l 
		enum class OperationResultType : uint8_t
		{
			kSuccess = 0U,
			kInsufficientMemory = 1U,
			kIncorrectBlock = 2U,
			kIncorrectSize = 3U,
			kInvalidTransferId = 4U,
			kOperationNotPermitted = 5U,
			/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
			kIncorrectBlockSize = 30U,
			kInsufficientData = 6U,
			/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
		};



		enum class TransferStartSuccessType : uint8_t
		{
			kSuccess = 0U,
			kInsufficientMemory = 1U,

		};


		typedef struct
		{
			TransferIdType id;		  // Size (in bytes) of the Software Package to be transferred
			uint32_t BlockSize;		  // Size of the blocks to be received with TransferData method
			TransferStartSuccessType TransferStartResult; // Success or Failure

		}TransferStartReturnType;


		enum class TransferDataReturnType :uint8_t
		{
			kSuccess = 0U,
			kIncorrectBlock = 1U,
			kIncorrectBlockSize = 2U,
			kInsufficientMemory = 3U,
			kInvalidTransferId = 4U,
			kOperationNotPermitted = 5U,

		};

		
		enum class TransferExitReturnType :uint8_t
		{
			kSuccess = 0U,
			kInsufficientData = 1U,
			kPackageInconsistent = 2U,
			kInvalidTransferId = 3U,
			kOperationNotPermitted = 4U,

		};
	}
}

