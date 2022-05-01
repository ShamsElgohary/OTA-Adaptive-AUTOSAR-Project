#include "SM.hpp"
#include "../exec/include/state_client.hpp"
using namespace ara::sm;
using namespace ara::exec;
using namespace std;

std::future<skeleton::UpdateRequestSkeleton::StartUpdateSessionOutput> UpdateRequestImpl::StartUpdateSession()
{
    StateClient client{};
    std::promise<UpdateRequestSkeleton::StartUpdateSessionOutput> promise;
    UpdateRequestSkeleton::StartUpdateSessionOutput out;
    bool success = client.setState(FunctionGroupState({"MachineState","Updating"}));
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
std::future<skeleton::UpdateRequestSkeleton::PrepareUpdateOutput> UpdateRequestImpl::PrepareUpdate(FunctionGroupList FunctionGroups)
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
        if (success)
        {
        out.AppError=success;
        this->FunctionGroupStates[fg]="Preparing";
        }
        else
        {
        printf("%s not prepared correctly\n",fg.c_str());    
        out.AppError=uint8_t(SM_ApplicationError::kPrepareFailed);
        break;
        }
    }
    }
    else
    {
        printf("StartUpdateSession must be called before\n");
        out.AppError=(uint8_t)SM_ApplicationError::kRejected;
    }
    promise.set_value(out);
    return promise.get_future();
}
std::future<skeleton::UpdateRequestSkeleton::VerifyUpdateOutput> UpdateRequestImpl::VerifyUpdate(FunctionGroupList FunctionGroups)
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
        printf("PrepareUpdate must be called before\n");
        out.AppError=uint8_t(SM_ApplicationError::kRejected);
        break;
        }
        success = client.setState(FunctionGroupState({fg, "Verifying"}));
        if (success)
        {
            out.AppError=success;
            this->FunctionGroupStates[fg]="Verifying";
        }
        else
        {
        printf("%s in not verifiyed correctly\n",fg.c_str());
        out.AppError=uint8_t(SM_ApplicationError::kVerifyFailed);
        break;
        }
    }
    
    }
    else
    {
        printf("StartUpdateSession must be called before\n");
        out.AppError=(uint8_t)SM_ApplicationError::kRejected;
    }
    promise.set_value(out);
    return promise.get_future();
}
