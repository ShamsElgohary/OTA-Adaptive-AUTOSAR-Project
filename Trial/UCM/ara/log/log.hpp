#pragma once

#include <iostream>
#include <fstream>


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
        void ActionsLog( string Action, unsigned char Result )
        {
            ofstream ReportAction("ActionsLog.txt", ios_base::out | ios_base::app);
            /* WRITE THE ACTION AND THE RESULT OF THIS ACTION */
            ReportAction << Action << OperationResult[ Result ] << '\n';
        }   

        private:

        /* USED TO CONVERT THE UCM OPERATION RESULTS INTO A STRING */
        string OperationResult[7] =
            { "kSuccess", "kInsufficientMemory", "kIncorrectBlock", "kIncorrectSize", "kInvalidTransferId", "kOperationNotPermitted", "kInsufficientData" };

    };
}

