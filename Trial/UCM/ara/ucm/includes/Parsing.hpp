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

				vector<string> SoftwareClusterManifestParser(string ClusterPath, string TransferID );

				vector<string> SoftwarePackageManifestParser(string ClusterPath, string TransferID );

				/* FILE PATH, ZIPPED FILE NAME IS OBTAINED FROM THE TRANSFER ID*/
				void UnzipFile(string FilePath, string TransferID );
			};
		}
	}
}

