#pragma once

#include "UCM_Common.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;
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
