#include "execution_client.h"
#include <iostream>
#include "SM.hpp"
using namespace std;
using namespace ara::sm;
using namespace ara::exec;

int main()
{
    printf("SM started to work");
    /********Report to EM*****/
    ExecutionState state = ExecutionState::KRunning;
    ExecutionClient client= ExecutionClient();
    client.ReportExecutionStaste(state);
    /********Test*************/
    UpdateRequest updaterequest=UpdateRequest();
    vector<Functiongroup>function_groups={"fg1","fg2"};
    updaterequest.StartUpdateSession();
    updaterequest.VerifyUpdate(function_groups);
    updaterequest.StopUpdateSession();
}