#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include "nlohmann/json.hpp"
#include "includes/UCM_Types.hpp"
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
        string command = "unzip " + FilePath + "TransferID:" + TransferID + ".zip";
        system(command.c_str());
    }

