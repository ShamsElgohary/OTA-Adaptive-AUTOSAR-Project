#include "includes/log.hpp"

using namespace ara;

void log::ReportError( string Error )
{
    ofstream ReportError("LogError.txt", ios_base::out | ios_base::app);
    ReportError << Error << '\n';
}


void log::ActionsLog( string Action, unsigned char Result )
{
    ofstream ReportAction("ActionsLog.txt", ios_base::out | ios_base::app);
    /* WRITE THE ACTION AND THE RESULT OF THIS ACTION */
    ReportAction << Action << UCM_Operation_str[ Result ] << '\n';
}

