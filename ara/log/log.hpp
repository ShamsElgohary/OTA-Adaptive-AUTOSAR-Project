#pragma once

#include <iostream>
#include <fstream>
#include "../../utility/jsoncpp/header/json.h"


using namespace std;

namespace ara
{
    class log
    {
        public:

        /* REPORTS ONLY ERRORS */
        void ReportError( string Error )
        {
            ofstream ReportError("LogError.txt", ios_base::out | ios_base::app);
            ReportError << Error << '\n';   
        }


        /* REPORTS ALL ACTIONS THAT OCCURED*/
        void ActionsLog( string Action, uint8_t Result )
        {
            try
            {
            ofstream ReportAction("ActionsLog.txt", ios_base::out | ios_base::app);
            /* WRITE THE ACTION AND THE RESULT OF THIS ACTION */
            ReportAction << Action << OperationResult[ Result ] << '\n';
            ReportAction.close();

            // GET METHOD CALLED
            uint8_t index = Action.find("]");
            string method = Action.substr(1,index-1); 

            // Read JSON File
            Json::Value event;
            ifstream inputFile("GUI_Report.json");
            Json::Reader R;
            R.parse(inputFile, event);

            event["ucm_json"]["PackageManager"][method]=Json::Value(OperationResult[Result]);

            std::ofstream json_file("GUI_Report.json");
            json_file<<event;
            json_file.close();

            }
            catch(const std::exception& e)
            {
                std::cout << e.what() << '\n';
            }
            
        }   

        void ReportStatus(uint8_t statusId) 
        {
            // Read JSON File
            Json::Value event;
            ifstream inputFile("GUI_Report.json");
            Json::Reader R;
            R.parse(inputFile, event);

            event["ucm_json"]["GUI"]["PackageManagerStatus"]=Json::Value(CurrentStatusTypes[statusId]);

            std::ofstream json_file("GUI_Report.json");
            json_file<<event;
            json_file.close();            

        }


        template <typename T>
        void ReportJsonGUI( string section, T value, bool ActivateCase = false )
        {
            
            // Read JSON File
            Json::Value event;
            ifstream inputFile("GUI_Report.json");
            Json::Reader R;
            R.parse(inputFile, event);

            if (!ActivateCase)
            {
                event["ucm_json"]["GUI"][section]=Json::Value(value);
            }
            else
            {
                event["ucm_json"]["GUI"]["Activate"][section]=Json::Value(value);
            }

            std::ofstream json_file("GUI_Report.json");
            json_file<<event;
            json_file.close();

        }




        private:

        /* USED TO CONVERT THE UCM OPERATION RESULTS INTO A STRING */
        string OperationResult[7] =
            { "kSuccess", "kInsufficientMemory", "kIncorrectBlock", "kIncorrectSize", "kInvalidTransferId", "kOperationNotPermitted", "kInsufficientData" };

        /* USED TO CONVERT THE UCM CURRENT STATUS RESULTS INTO A STRING */        
        string CurrentStatusTypes[9] =
            { "kIdle", "kReady", "kProcessing", "kActivating", "kActivated", "kRollingBack", "kRolledBack", "kCleaningUp", "kVerifying" };


    };
}

