#include "includes/Parsing.hpp"

using namespace ara::ucm::parsing;


SwClusterInfoType SoftwarePackageParser::GetSwClusterInfo(string PackagePath)
{
    SwClusterInfoType swClusterInfo;

    string Path = PackagePath + "/" + "swclustermanifest.json";
    // Create a root
    pt::ptree root;
    // Load the json file in this ptree
    pt::read_json(Path, root);
    string version = root.get<std::string>("version");
    string name = root.get<std::string>("shortName");

    swClusterInfo.Version = version;
    swClusterInfo.Name = name;

    return swClusterInfo;
}

void SoftwarePackageParser::SwPackageManifestParser(string PackagePath)
{
string Path = PackagePath + "/" + "swpackagemanifest.json";
// Create a root
pt::ptree root;
// Load the json file in this ptree
pt::read_json(Path, root);


string actionTypeString { root.get<string>("actionType", 0) };

if ( actionTypeString == "Install")
{  
    actionType = ara::ucm::ActionType::kInstall;
}

else if( actionTypeString == "Update" )
{
    actionType = ara::ucm::ActionType::kUpdate;   
}

else if( actionTypeString == "Remove" )
{
    actionType = ara::ucm::ActionType::kUpdate;   
}

activationAction = root.get<std::string>("activationAction");
deltaPackageApplicableVersion = root.get<std::string>("deltaPackageApplicableVersion");
}

ActionType SoftwarePackageParser::GetActionType()
{
return actionType;
}

string SoftwarePackageParser::GetActivationAction()
{
return activationAction;
}

string SoftwarePackageParser::GetDeltaPackageApplicableVersion()
{
return deltaPackageApplicableVersion;
}

void SoftwarePackageParser::UnzipFile(string FilePath)
{
    string command = "unzip " + FilePath + ".zip";
    system(command.c_str());
}

