#pragma once

#include <iostream>
#include <fstream>
#include "UCM_Common.hpp"

using namespace std;

namespace ara
{
    class log
    {
        public:

        /* REPORTS ONLY ERRORS */
        void ReportError( string Error );

        /* REPORTS ALL ACTIONS THAT OCCURED*/
        void ActionsLog( string Action, unsigned char Result );

        private:

        /* USED TO CONVERT THE UCM OPERATION RESULTS INTO A STRING */
        string UCM_Operation_str[7] =
            { "kSuccess", "kInsufficientMemory", "kIncorrectBlock", "kIncorrectSize", "kInvalidTransferId", "kOperationNotPermitted", "kInsufficientData" };

    };
}

