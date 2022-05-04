#include "SM.hpp"
#include "../exec/include/state_client.hpp"
#include "../../utility/simulation/include/sm_logger.hpp"
using namespace ara::sm;
using namespace ara::exec;
using namespace std;

std::future<skeleton::UpdateRequestSkeleton::StartUpdateSessionOutput> UpdateRequestImpl::StartUpdateSession()
{
    StateClient client{};
    std::promise<UpdateRequestSkeleton::StartUpdateSessionOutput> promise;
    UpdateRequestSkeleton::StartUpdateSessionOutput out;
    FunctionGroupState state=FunctionGroupState::Preconstruct("machineFG","Updating");
    bool success = client.setState(state);
    if(success)
    {
    this->FunctionGroupStates[state.fg_name]=state.fg_newState;
    out.AppError=success;
    }
    else
    out.AppError=uint8_t(SM_ApplicationError::kRejected);

    promise.set_value(out);
    
    sm_logger log(8088);
    sm_functions functions={.sm_StartUpdateSession=(1),.sm_StopUpdateSession=(0),.sm_PrepareUpdate=(0),.sm_VerifyUpdate=(0)};
    log.update_logger(functions,this->FunctionGroupStates);
    
    return promise.get_future();
}
void UpdateRequestImpl::StopUpdateSession()
{
    StateClient client{};
    FunctionGroupState state=FunctionGroupState::Preconstruct("machineFG", "running");
    bool success = client.setState(state);
    if(success)
    this->FunctionGroupStates[state.fg_name]=state.fg_newState;
    sm_logger log(8088);
    sm_functions functions={.sm_StartUpdateSession=(0),.sm_StopUpdateSession=(1),.sm_PrepareUpdate=(0),.sm_VerifyUpdate=(0)};
    log.update_logger(functions,this->FunctionGroupStates);
}
std::future<skeleton::UpdateRequestSkeleton::PrepareUpdateOutput> UpdateRequestImpl::PrepareUpdate(FunctionGroupList FunctionGroups)
{
    bool success;
    StateClient client{};
    std::promise<UpdateRequestSkeleton::PrepareUpdateOutput> promise;
    UpdateRequestSkeleton::PrepareUpdateOutput out;
    if(this->FunctionGroupStates["machineFG"]=="Updating")
    {
    for (auto fg : FunctionGroups)
    {
        FunctionGroupState state=FunctionGroupState::Preconstruct(fg,"off");
        success = client.setState(state);
        if (success)
        {
        out.AppError=success;
        this->FunctionGroupStates[state.fg_name]=state.fg_newState;
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

    sm_logger log(8088);
    sm_functions functions={.sm_StartUpdateSession=(0),.sm_StopUpdateSession=(0),.sm_PrepareUpdate=(1),.sm_VerifyUpdate=(0)};
    log.update_logger(functions,this->FunctionGroupStates);

    return promise.get_future();
}
std::future<skeleton::UpdateRequestSkeleton::VerifyUpdateOutput> UpdateRequestImpl::VerifyUpdate(FunctionGroupList FunctionGroups)
{
    bool success;
    StateClient client{};
    std::promise<UpdateRequestSkeleton::VerifyUpdateOutput> promise;
    UpdateRequestSkeleton::VerifyUpdateOutput out;
    if(this->FunctionGroupStates["machineFG"]=="Updating")
    {
    for (auto fg : FunctionGroups)
    {
        if(this->FunctionGroupStates[fg]!="off")
        {
        printf("PrepareUpdate must be called before\n");
        out.AppError=uint8_t(SM_ApplicationError::kRejected);
        break;
        }
        FunctionGroupState state = FunctionGroupState::Preconstruct(fg,"verify");
        success = client.setState(state);
        if (success)
        {
            out.AppError=success;
            this->FunctionGroupStates[state.fg_name]=state.fg_newState;
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
    
    sm_logger log(8088);
    sm_functions functions={.sm_StartUpdateSession=(0),.sm_StopUpdateSession=(0),.sm_PrepareUpdate=(0),.sm_VerifyUpdate=(1)};
    log.update_logger(functions,this->FunctionGroupStates);

    return promise.get_future();
}
