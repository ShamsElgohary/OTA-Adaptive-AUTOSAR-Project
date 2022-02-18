#include "SM.hpp"
#include "../exec/include/state_client.hpp"
using namespace ara::sm;
using namespace ara::exec;
using namespace std;

uint8_t UpdateRequest::StartUpdateSession()
{
    StateClient client{};
    bool success = client.setState(FunctionGroupState({"MachineState", "Updating"}));
    if(success) return success;
    else return uint8_t(SM_ApplicationError::kRejected);
}
void UpdateRequest::StopUpdateSession()
{
    StateClient client{};
    bool success = client.setState(FunctionGroupState({"MachineState", "Running"}));
}
uint8_t UpdateRequest::PrepareUpdate(FunctionGroupList FunctionGroups)
{
    bool success;
    StateClient client{};
    //if(client.getmachinestate()=="Updating")
    //{
    for (auto fg : FunctionGroups)
    {
        success = client.setState(FunctionGroupState({fg, "Preparing"}));
        if (!success)
            return uint8_t(SM_ApplicationError::kPrepareFailed);
    }
    return success;
    //}
    //else
    //{
      //  throw ("StartUpdateSession must be called before");
        //return (uint8_t)SM_ApplicationError::kRejected;
    //}
}
uint8_t UpdateRequest::VerifyUpdate(FunctionGroupList FunctionGroups)
{
    bool success;
    StateClient client{};
    for (auto fg : FunctionGroups)
    {
        success = client.setState(FunctionGroupState({fg, "Verifying"}));
        if (!success)
            return  uint8_t(SM_ApplicationError::kVerifyFailed);
    }
    return success;
}
