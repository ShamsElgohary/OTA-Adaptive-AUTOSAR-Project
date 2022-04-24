#include "SM.hpp"
#include "../exec/include/state_client.hpp"
using namespace ara::sm;
using namespace ara::exec;
using namespace std;

std::future<UpdateRequestSkeleton::StartUpdateSessionOutput> UpdateRequestImpl::StartUpdateSession()
{
    StateClient client{};
    std::promise<UpdateRequestSkeleton::StartUpdateSessionOutput> promise;
    UpdateRequestSkeleton::StartUpdateSessionOutput out;
    bool success = client.setState(FunctionGroupState({"MachineState", "Updating"}));
    if(success)
    {
    this->FunctionGroupStates["MachineState"]="Updating";
    out.AppError=success;
    }
    else
    out.AppError=uint8_t(SM_ApplicationError::kRejected);

    promise.set_value(out);
    return promise.get_future();
}
void UpdateRequestImpl::StopUpdateSession()
{
    StateClient client{};
    bool success = client.setState(FunctionGroupState({"MachineState", "Running"}));
    this->FunctionGroupStates["MachineState"]="Running";
}
std::future<UpdateRequestSkeleton::PrepareUpdateOutput> UpdateRequestImpl::PrepareUpdate(FunctionGroupList FunctionGroups)
{
    bool success;
    StateClient client{};
    std::promise<UpdateRequestSkeleton::PrepareUpdateOutput> promise;
    UpdateRequestSkeleton::PrepareUpdateOutput out;
    if(this->FunctionGroupStates["MachineState"]=="Updating")
    {
    for (auto fg : FunctionGroups)
    {
        success = client.setState(FunctionGroupState({fg, "Preparing"}));
        if (!success)
            out.AppError=uint8_t(SM_ApplicationError::kPrepareFailed);
        this->FunctionGroupStates[fg]="Preparing";
    }
    out.AppError=success;
    }
    else
    {
        throw std::string("StartUpdateSession must be called before\n");
        out.AppError=(uint8_t)SM_ApplicationError::kRejected;
    }
    promise.set_value(out);
    return promise.get_future();
}
std::future<UpdateRequestSkeleton::VerifyUpdateOutput> UpdateRequestImpl::VerifyUpdate(FunctionGroupList FunctionGroups)
{
    bool success;
    StateClient client{};
    std::promise<UpdateRequestSkeleton::VerifyUpdateOutput> promise;
    UpdateRequestSkeleton::VerifyUpdateOutput out;
    if(this->FunctionGroupStates["MachineState"]=="Updating")
    {
    for (auto fg : FunctionGroups)
    {
        if(this->FunctionGroupStates[fg]!="Preparing")
        {
        throw std::string("PrepareUpdate must be called before");
        out.AppError=uint8_t(SM_ApplicationError::kRejected);
        }
        success = client.setState(FunctionGroupState({fg, "Verifying"}));
        if (!success)
        out.AppError=uint8_t(SM_ApplicationError::kVerifyFailed);
        this->FunctionGroupStates[fg]="Verifying";
    }
    out.AppError=success;
    }
    else
    {
        throw std::string("StartUpdateSession must be called before\n");
        out.AppError=(uint8_t)SM_ApplicationError::kRejected;
    }
    promise.set_value(out);
    return promise.get_future();
}
