#include "SM.hpp"
#include "state_client.h"
using namespace ara::sm;
using namespace ara::exec;
using namespace std;

bool UpdateRequest::StartUpdateSession()
{
    StateClient client=StateClient();                                         
    bool success = client.setState(FunctionGroupState({"MachineState", "Updating"})); 
    return success;
}
bool UpdateRequest::StopUpdateSession()
{
    StateClient client=StateClient();                                       
    bool success = client.setState(FunctionGroupState({"MachineState", "Running"})); 
    return success;
}
bool UpdateRequest::PrepareUpdate(vector<Functiongroup>FunctionGroups)
{
    bool success;
    for (auto fg : FunctionGroups)
    {
        StateClient client=StateClient();                                   
        success = client.setState(FunctionGroupState({fg, "Preparing"})); 
        if (!success)
            return 0;
    }
    return success;
}
bool UpdateRequest::VerifyUpdate(vector<Functiongroup> FunctionGroups)
{
    bool success;
    for (auto fg : FunctionGroups)
    {
        StateClient client=StateClient();                                 
        success = client.setState(FunctionGroupState({fg, "Verifying"})); 
        if (!success)
            return 0;
    }
    return success;
}
