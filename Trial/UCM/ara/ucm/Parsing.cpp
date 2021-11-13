#include "includes/Parsing.hpp"

using namespace ara::ucm::parsing;


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

