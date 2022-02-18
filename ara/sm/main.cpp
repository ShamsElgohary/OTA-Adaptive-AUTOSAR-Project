#include "../exec/include/execution_client.hpp"
#include <iostream>
#include "SM.hpp"
using namespace std;
using namespace ara::sm;
using namespace ara::exec;

int main()
{
    printf("SM started to work\n");
    /********Report to EM*****/
    ExecutionClient client{};
    client.ReportExecutionStaste(ExecutionState::KRunning);
    /********Test*************/
    UpdateRequest updaterequest{};
    FunctionGroupList function_groups = {"fg1", "fg2"};
    updaterequest.StartUpdateSession();
    updaterequest.VerifyUpdate(function_groups);
    updaterequest.StopUpdateSession();
}