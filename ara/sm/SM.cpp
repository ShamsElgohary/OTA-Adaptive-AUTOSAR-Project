#include "SM.hpp"
#include "../exec/include/state_client.hpp"
using namespace ara::sm;
using namespace ara::exec;
using namespace std;

uint8_t UpdateRequest::StartUpdateSession()
{
    StateClient client{};
    bool success = client.setState(FunctionGroupState({"MachineState", "Updating"}));
    if(success)
    {
    this->FunctionGroupStates["MachineState"]="Updating";
    return success;
    }
    else return uint8_t(SM_ApplicationError::kRejected);
}
void UpdateRequest::StopUpdateSession()
{
    StateClient client{};
    bool success = client.setState(FunctionGroupState({"MachineState", "Running"}));
    this->FunctionGroupStates["MachineState"]="Running";
}
uint8_t UpdateRequest::PrepareUpdate(FunctionGroupList FunctionGroups)
{
    bool success;
    StateClient client{};
    if(this->FunctionGroupStates["MachineState"]=="Updating")
    {
    for (auto fg : FunctionGroups)
    {
        success = client.setState(FunctionGroupState({fg, "Preparing"}));
        if (!success)
            return uint8_t(SM_ApplicationError::kPrepareFailed);
        this->FunctionGroupStates[fg]="Preparing";
    }
    return success;
    }
    else
    {
        throw std::string("StartUpdateSession must be called before\n");
        return (uint8_t)SM_ApplicationError::kRejected;
    }
}
uint8_t UpdateRequest::VerifyUpdate(FunctionGroupList FunctionGroups)
{
    bool success;
    StateClient client{};
    if(this->FunctionGroupStates["MachineState"]=="Updating")
    {
    for (auto fg : FunctionGroups)
    {
        if(this->FunctionGroupStates[fg]!="Preparing")
        {
        throw std::string("PrepareUpdate must be called before");
        return uint8_t(SM_ApplicationError::kRejected);
        }
        success = client.setState(FunctionGroupState({fg, "Verifying"}));
        if (!success)
            return  uint8_t(SM_ApplicationError::kVerifyFailed);
        this->FunctionGroupStates[fg]="Verifying";
    }
    return success;
    }
    else
    {
        throw std::string("StartUpdateSession must be called before\n");
        return (uint8_t)SM_ApplicationError::kRejected;
    }
}
