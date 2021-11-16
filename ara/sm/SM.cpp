#include "SM.hpp"

using namespace ara::sm;
using namespace std;

bool UpdateRequest::StartUpdateSession()
{
    StateClient client();                                         
    bool success = client.SetState({"MachineState", "Updating"}); 
    return success;
}
bool UpdateRequest::StopUpdateSession()
{
    StateClient client();                                        
    bool success = client.SetState({"MachineState", "Running"}); 
    return success;
}
bool UpdateRequest::PrepareUpdate(vector<FunctionGroup>FunctionGroups)
{
    bool success;
    for (auto fg : FunctionGroups)
    {
        StateClient client();                                   
        success = client.SetState({fg, "Preparing"}); 
        if (!success)
            return 0;
    }
    return success;
}
bool UpdateRequest::VerifyUpdate(vector<FunctionGroup> FunctionGroups)
{
    bool success;
    for (auto fg : FunctionGroups)
    {
        StateClient client();                                   
        success = client.SetState({fg, "Verifying"}); 
        if (!success)
            return 0;
    }
    return success;
}
