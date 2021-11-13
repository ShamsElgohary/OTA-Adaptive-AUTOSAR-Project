#pragma once

#include "UCM_Common.hpp"

namespace ara
{
	namespace ucm
	{
		namespace parsing
		{
			class SoftwarePackageParser
			{
			private:



			public:

				SwClusterInfoType GetSwClusterInfo(string PackagePath, string TransferID);
				void SoftwarePackageParser::SwPackageManifestParser(string PackagePath, string TransferID);
				ActionType GetActionType();
				string GetActivationAction();
				string GetDeltaPackageApplicableVersion();
				/* FILE PATH, ZIPPED FILE NAME IS OBTAINED FROM THE TRANSFER ID*/
				void UnzipFile(string FilePath, string TransferID);
			};
		}
	}
}

