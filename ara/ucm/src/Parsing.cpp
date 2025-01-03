#include "../includes/Parsing.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

using namespace ara::ucm::parsing;
using namespace std;

SwClusterInfoType SoftwarePackageParser::GetSwClusterInfo(string PackagePath)
{
    SwClusterInfoType swClusterInfo;

    string Path = PackagePath + "/SoftwareCluster/SwClusterManifest.json";
    // Create a root
    pt::ptree root;
    // Load the json file in this ptree
    pt::read_json(Path, root);
    string version = root.get<std::string>("version").substr(0, 3);
    string name = root.get<std::string>("shortName");

    swClusterInfo.Version = version;
    swClusterInfo.Name = name;

    GUI_Logger.newPkgCluster(name, version);

    return swClusterInfo;
}

void SoftwarePackageParser::SwPackageManifestParser(string PackagePath)
{
    string Path = PackagePath + "/SwPackageManifest.json";
    // Create a root
    pt::ptree root;
    // Load the json file in this ptree
    pt::read_json(Path, root);
    
    string actionTypeString{root.get<string>("actionType")};
    std::for_each(actionTypeString.begin(), actionTypeString.end(), [](char &c)
                  { c = ::tolower(c); });
    if (actionTypeString == "install")
    {
        actionType = ara::ucm::ActionType::kInstall;
    }

    else if (actionTypeString == "update")
    {
        actionType = ara::ucm::ActionType::kUpdate;
    }

    else if (actionTypeString == "remove")
    {
        actionType = ara::ucm::ActionType::kRemove;
    }

    activationAction = root.get<std::string>("activationAction");

    GUI_Logger.pkgAction(actionTypeString);

    // LOGGING & GUI SIMULATION
    GUI_Logger.ReportJsonGUI("ProcessedActionType", actionTypeString);

    // deltaPackageApplicableVersion = root.get<std::string>("deltaPackageApplicableVersion");
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

string SoftwarePackageParser::UnzipPackage(string SoftwarePackage)
{
    /* PACKAGE WILL BE A DIRECTORY NOT A ZIPPED FILE  (DISREGARD .ZIP) */
    string UnzippedPath{SoftwarePackage.substr(0, SoftwarePackage.length() - 4)};

    /* If Path Doesn't Exist,Then Make Directory */
    if (IsPathExist(UnzippedPath.c_str()) == false)
    {
        command = "mkdir " + UnzippedPath;
        system(&command[0]);
    }

    std::string latestCurrentDirectory = GetCurrentDirectory();

    /* UNZIP IN NEW PATH ( DIRECTORY WITH THE SAME NAME (TRANSFER ID : --) ) */
    chdir(UnzippedPath.c_str());
    command = "unzip " + SoftwarePackage;
    system(command.c_str());

    /* REMOVE TRANSFERED ZIPPED FILE */
    command = "rm -r " + SoftwarePackage;
    system(command.c_str());

    /* RETURN TO THE PARENT PROCESS */
    chdir(latestCurrentDirectory.c_str());

    return UnzippedPath;
}
