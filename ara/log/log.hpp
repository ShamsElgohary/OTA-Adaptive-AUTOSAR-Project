#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "../ucm/includes/types.hpp"
#include "../../utility/jsoncpp/header/json.h"
#include "../../utility/simulation/include/simulation.hpp"
#include "../../utility/general.hpp"

using namespace std;

namespace ara
{
    class log
    {
    public:
        /* REPORTS ONLY ERRORS */
        void ReportError(string Error)
        {
            ofstream ReportError("LogError.txt", ios_base::out | ios_base::app);
            ReportError << Error << '\n';
        }

        /* REPORTS ALL ACTIONS THAT OCCURED*/
        void ActionsLog(string Action, uint8_t Result)
        {
            try
            {
                ofstream ReportAction("ActionsLog.txt", ios_base::out | ios_base::app);
                /* WRITE THE ACTION AND THE RESULT OF THIS ACTION */
                ReportAction << Action << OperationResult[Result] << '\n';
                ReportAction.close();

                // GET METHOD CALLED
                uint8_t index = Action.find("]");
                string method = Action.substr(1, index - 1);

                // Read JSON File
                Json::Value event;
                ifstream inputFile("Gui.json");
                Json::Reader R;
                R.parse(inputFile, event);

                event["ucm_json"]["PackageManager"][method] = Json::Value(OperationResult[Result]);

                std::ofstream json_file("Gui.json");
                json_file << event;
                json_file.close();
            }
            catch (const std::exception &e)
            {
                std::cout << e.what() << '\n';
            }

            if (SIMULATION_ACTIVE)
            {
                std::string Path = CUSTOMIZED_PROJECT_PATH + "/executables/ucm/bin/Gui.json";
                char PathArr[Path.length() + 1]; 
                guiSocket->connect_to_socket();
                guiSocket->send_file(PathArr);
            }
        }

        void pkgAction(string actiontype)
        {
            // Read JSON File
            Json::Value event;
            ifstream inputFile("Gui.json");
            Json::Reader R;
            R.parse(inputFile, event);

            event["ucm_json"]["PKGDetails"]["Action"] = Json::Value(actiontype);

            std::ofstream json_file("Gui.json");
            json_file << event;
            json_file.close();

            if (SIMULATION_ACTIVE)
            {
                std::string Path = CUSTOMIZED_PROJECT_PATH + "/executables/ucm/bin/Gui.json";
                char PathArr[Path.length() + 1]; 
                guiSocket->connect_to_socket();
                guiSocket->send_file(PathArr);
            }
        }

        void newPkgCluster(string name, string version)
        {
            // Read JSON File
            Json::Value event;
            ifstream inputFile("Gui.json");
            Json::Reader R;
            R.parse(inputFile, event);

            event["ucm_json"]["PKGDetails"]["Clusters"]["sw1"]["name"] = name;
            event["ucm_json"]["PKGDetails"]["Clusters"]["sw1"]["version"] = version;

            std::ofstream json_file("Gui.json");
            json_file << event;
            json_file.close();

            if (SIMULATION_ACTIVE)
            {
                std::string Path = CUSTOMIZED_PROJECT_PATH + "/executables/ucm/bin/Gui.json";
                char PathArr[Path.length() + 1]; 
                guiSocket->connect_to_socket();
                guiSocket->send_file(PathArr);
            }
        }

        void ReportStatus(uint8_t statusId)
        {
            // Read JSON File
            Json::Value event;
            ifstream inputFile("Gui.json");
            Json::Reader R;
            R.parse(inputFile, event);

            event["ucm_json"]["GUI"]["PackageManagerStatus"] = Json::Value(CurrentStatusTypes[statusId]);

            std::ofstream json_file("Gui.json");
            json_file << event;
            json_file.close();

            if (SIMULATION_ACTIVE)
            {
                std::string Path = CUSTOMIZED_PROJECT_PATH + "/executables/ucm/bin/Gui.json";
                char PathArr[Path.length() + 1]; 
                guiSocket->connect_to_socket();
                guiSocket->send_file(PathArr);
            }
        }

        template <typename T>
        void ReportJsonGUI(string section, T value, bool ActivateCase = false)
        {

            // Read JSON File
            Json::Value event;
            ifstream inputFile("Gui.json");
            Json::Reader R;
            R.parse(inputFile, event);

            if (!ActivateCase)
            {
                event["ucm_json"]["GUI"][section] = Json::Value(value);
            }
            else
            {
                event["ucm_json"]["GUI"]["Activate"][section] = Json::Value(value);
            }

            std::ofstream json_file("Gui.json");
            json_file << event;
            json_file.close();
            if (SIMULATION_ACTIVE)
            {
                std::string Path = CUSTOMIZED_PROJECT_PATH + "/executables/ucm/bin/Gui.json";
                char PathArr[Path.length() + 1]; 
                guiSocket->connect_to_socket();
                guiSocket->send_file(PathArr);
            }
        }

        void ReportPresentSWClusters(vector<ara::ucm::SwClusterInfoType> presentClusters)
        {
            // Read JSON File
            Json::Value event;
            ifstream inputFile("Gui.json");
            Json::Reader R;
            R.parse(inputFile, event);

            std::string swClusterString = "Cluster";
            char clusterNumb = '1';

            for (int i = 0; i < presentClusters.size(); i++)
            {
                std::string swClusterIndex = swClusterString + clusterNumb;
                event["ucm_json"]["PresentClusters"][swClusterIndex]["Name"] = Json::Value(presentClusters[i].Name);
                event["ucm_json"]["PresentClusters"][swClusterIndex]["Version"] = Json::Value(presentClusters[i].Version);
                clusterNumb++;
            }

            std::ofstream json_file("Gui.json");
            json_file << event;
            json_file.close();
            if (SIMULATION_ACTIVE)
            {
                std::string Path = CUSTOMIZED_PROJECT_PATH + "/executables/ucm/bin/Gui.json";
                char PathArr[Path.length() + 1]; 
                guiSocket->connect_to_socket();
                guiSocket->send_file(PathArr);
            }
        }

    private:
        simulation *guiSocket = new simulation(8088);

        /* USED TO CONVERT THE UCM OPERATION RESULTS INTO A STRING */
        string OperationResult[7] =
            {"kSuccess", "kInsufficientMemory", "kIncorrectBlock", "kIncorrectSize", "kInvalidTransferId", "kOperationNotPermitted", "kInsufficientData"};

        /* USED TO CONVERT THE UCM CURRENT STATUS RESULTS INTO A STRING */
        string CurrentStatusTypes[9] =
            {"kIdle", "kReady", "kProcessing", "kActivating", "kActivated", "kRollingBack", "kRolledBack", "kCleaningUp", "kVerifying"};
    };
}
