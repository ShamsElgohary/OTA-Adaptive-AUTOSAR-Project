#pragma once

#include "UCM_Common.hpp"

using namespace ara::ucm;


namespace ara
{
	namespace ucm
	{
		namespace parsing
		{
			class SoftwarePackageParser
			{
			private:
				ActionType actionType;
				string activationAction;
				string deltaPackageApplicableVersion;

			public:
				SwClusterInfoType GetSwClusterInfo(string PackagePath);
				void SwPackageManifestParser(string PackagePath);
				ActionType GetActionType();
				string GetActivationAction();
				string GetDeltaPackageApplicableVersion();
				string UnzipPackage(string SoftwarePackage);
			};
		}
	}
}
