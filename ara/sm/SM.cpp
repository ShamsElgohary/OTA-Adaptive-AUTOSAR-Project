#include "SM.hpp"
#include "state_client.h"
using namespace ara::sm;
using namespace ara::exec;
using namespace std;

bool UpdateRequest::StartUpdateSession()
{
    StateClient client{};                                         
    bool success = client.setState(FunctionGroupState({"MachineState", "Updating"})); 
    return success;
}
bool UpdateRequest::StopUpdateSession()
{
    StateClient client{};                                       
    bool success = client.setState(FunctionGroupState({"MachineState", "Running"})); 
    return success;
}
bool UpdateRequest::PrepareUpdate(vector<Functiongroup>FunctionGroups)
{
    bool success;
    StateClient client{}; 
    for (auto fg : FunctionGroups)
    {                                  
        success = client.setState(FunctionGroupState({fg, "Preparing"})); 
        if (!success)
            return 0;
    }
    return success;
}
bool UpdateRequest::VerifyUpdate(vector<Functiongroup> FunctionGroups)
{
    bool success;
    StateClient client{};  
    for (auto fg : FunctionGroups)
    {                               
        success = client.setState(FunctionGroupState({fg, "Verifying"})); 
        if (!success)
            return 0;
    }
    return success;
}
