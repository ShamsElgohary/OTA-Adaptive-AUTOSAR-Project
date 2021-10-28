 /******************************************************************************
 *
 * Functional Cluster : UCM
 *
 * File Name: UCM_Types.h
 *
 * Description: Header file that lists all types provided by the UCM.
 *
 ******************************************************************************/

#ifndef UCM_TYPES_H_
#define UCM_TYPES_H_

#include <iostream>
#include <vector>

using namespace std;



/* UCM Module Instantiation Identifier */
typedef string UCMIdentifierType;

/* Represents a handle identifier used to reference a particular transfer request */
typedef uint8_t TransferIdType[16];

/* SW Cluster or SW Package shortName attribute inherited from referrable metaClass */
typedef string SwNameType;

/* Represents a dynamic size array of Software Cluster names */
typedef vector<SwNameType> SwNameVectorType;

/* Primitive type representing SoftwareCluster (SoftwarePackage) version */
typedef string StrongRevisionLabelString;

/* Represents the information of a Software Package (Software Cluster) name and version */
typedef struct
{
	SwNameType Name;
	StrongRevisionLabelString Version;
}SwNameVersionType;

/* Represents a dynamic size array of Software Name and Version */
typedef vector<SwNameVersionType> SwNameVersionVectorType;

/* Byte vector representing raw data */
typedef vector<uint8_t> ByteVectorType;

/* Represents the state of a Software Package on the Platform  */
typedef uint8_t& SwPackageStateType;																		// NEED TO CHECK

/* Represents the information of a Software Package */
typedef struct
{
	SwNameType Name;
	StrongRevisionLabelString Version;
	TransferIdType TransferID;
	uint64_t ConsecutiveBytesReceived;
	uint64_t ConsecutiveBlocksReceived;
	SwPackageStateType State;
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
typedef uint8_t& SwClusterStateType;

/* Represents the information of a SoftwareCluster */
typedef struct
{
	SwNameType Name;
	StrongRevisionLabelString Version;
	SwClusterStateType State;
}SwClusterInfoType;

/* Represents a dynamic size array of SoftwareClusters */
typedef vector<SwClusterInfoType> SwClusterInfoVectorType;

/* Represents the state of UCM */
typedef uint8_t& PackageManagerStatusType;

/* Represents the UCM action */
typedef uint8_t& ActionType;

/* Represents the result of UCM action */
typedef uint8_t& ResultType;

/* Time refers to the activation time of the software cluster. It is represented in milliseconds
 * of UCM’s action resolution since 01.01.1970 (UTC).  */
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
typedef uint8_t& CampaignFailureType;


/* UCM Software Package step at which error occured */
typedef uint8_t& SoftwarePackageStepType;

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

typedef struct
{
	CampaignErrorType CampaignError;
	HistoryVectorType HistoryVector;
} CampaignHistoryType ;

/* Represents the status of Campaign */
typedef uint8_t& CampaignStateType;

/* Represents the state of an update from OTA Client perspective.Represents the state of an update from OTA Client perspective */
typedef uint8_t& TransferStateType;


/* The type of the Safety Policy */
typedef string SafetyPolicyType;

#endif /* UCM_TYPES_H_ */
