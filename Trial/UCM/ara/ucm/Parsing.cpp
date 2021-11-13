#include "includes/Parsing.hpp"

using namespace ara::ucm::parsing;

SwClusterInfoType swClusterInfo;
ActionType actionType;
string activationAction;
string deltaPackageApplicableVersion;


SwClusterInfoType GetSwClusterInfo(string PackagePath, string TransferID)
{
	string Path = PackagePath + "/" + TransferID + "/" + "swclustermanifest.json";
	// Create a root
	pt::ptree root;
	// Load the json file in this ptree
	pt::read_json(Path, root);
	string version = root.get<std::string>("version");
	string name = root.get<std::string>("shortName");

	swClusterInfo.Version = version;
	swClusterInfo.Name = name;
}

void SoftwarePackageParser::SwPackageManifestParser(string PackagePath, string TransferID)
{
	string Path = PackagePath + "/" + TransferID + "/" + "swpackagemanifest.json";
	// Create a root
	pt::ptree root;
	// Load the json file in this ptree
	pt::read_json(Path, root);
	actionType = root.get<uint8_t>("actionType", 0);
	activationAction = root.get<std::string>("activationAction");
	deltaPackageApplicableVersion = root.get<std::string>("deltaPackageApplicableVersion");
}

ActionType GetActionType()
{
	return actionType;
}

string GetActivationAction()
{
	return activationAction;
}

string GetDeltaPackageApplicableVersion()
{
	return deltaPackageApplicableVersion;
}
};

    vector<string> SoftwarePackageParser::SoftwareClusterManifestParser(string ClusterPath, string TransferID )
    {

    }

    vector<string> SoftwarePackageParser::SoftwarePackageManifestParser(string ClusterPath,  string TransferID )
    {

    }

    /* FILE PATH INCLUDES FILE NAME */
    void SoftwarePackageParser::UnzipFile(string FilePath, string TransferID )
    {
        string ParentPath = GetCurrentDirectory();
        chdir(ParentPath.c_str());
        string command = "unzip " + FilePath + "TransferID:" + TransferID + ".zip";
        system(command.c_str());
    }

