#include "SM.hpp"
using namespace ara::sm;
using namespace ara::exec;
using namespace std;
std::map<Functiongroup, std::string> UpdateRequestImpl::FunctionGroupStates;
std::future<UpdateRequest::UpdateRequestSkeleton::StartUpdateSessionOutput> UpdateRequestImpl::StartUpdateSession()
{
    sm_functions functions = {.sm_StartUpdateSession = (1), .sm_StopUpdateSession = (0), .sm_PrepareUpdate = (0), .sm_VerifyUpdate = (0)};
    std::cout << "Starting Update Session" << std::endl;

    std::promise<UpdateRequestSkeleton::StartUpdateSessionOutput> promise;
    UpdateRequestSkeleton::StartUpdateSessionOutput out;
    FunctionGroupState state = FunctionGroupState::Preconstruct("machineFG", "update");
    bool success = client->setState(state);
    if (success)
    {
        UpdateRequestImpl::FunctionGroupStates[state.fg_name] = "update";
        out.AppError = ara::sm::SM_ApplicationError::kPrepared;
        std::cout << state.fg_name << " now is in state " << state.fg_newState << std::endl;
    }
    else
    {
        out.AppError  = ara::sm::SM_ApplicationError::kRejected;
        std::cout << "Updating Machine is not allowed" << std::endl;
    }

    promise.set_value(out);

    log->update_logger(functions, FunctionGroupStates);

    return promise.get_future();
}
void UpdateRequestImpl::StopUpdateSession()
{
    cout << "Stop Updating Session" << std::endl;
    sm_functions functions = {.sm_StartUpdateSession = (0), .sm_StopUpdateSession = (1), .sm_PrepareUpdate = (0), .sm_VerifyUpdate = (0)};
    
    FunctionGroupState state = FunctionGroupState::Preconstruct("machineFG", "running");
    bool success = UpdateRequestImpl::client->setState(state);
    if (success)
        this->FunctionGroupStates[state.fg_name] = state.fg_newState;
    log->update_logger(functions, this->FunctionGroupStates);
    FunctionGroupState state2 = FunctionGroupState::Preconstruct("fn1", "idle");
    success = UpdateRequestImpl::client->setState(state2);
    if (success)
        this->FunctionGroupStates[state2.fg_name] = state2.fg_newState;
    log->update_logger(functions, this->FunctionGroupStates);
}
std::future<UpdateRequest::UpdateRequestSkeleton::PrepareUpdateOutput> UpdateRequestImpl::PrepareUpdate(FunctionGroupList FunctionGroups)
{
    cout << "Preparing Update" << std::endl;
    sm_functions functions = {.sm_StartUpdateSession = (0), .sm_StopUpdateSession = (0), .sm_PrepareUpdate = (1), .sm_VerifyUpdate = (0)};

    bool success;
    StateClient client{};
    std::promise<UpdateRequestSkeleton::PrepareUpdateOutput> promise;
    UpdateRequestSkeleton::PrepareUpdateOutput out;
    if (this->FunctionGroupStates["machineFG"] == "update")
    {
        for (auto fg : FunctionGroups)
        {
            FunctionGroupState state = FunctionGroupState::Preconstruct(fg, "off");
            success = client.setState(state);
            if (success)
            {
                std::cout << state.fg_name << " now is in state " << state.fg_newState << std::endl;
                out.AppError = ara::sm::SM_ApplicationError::kPrepared;
                this->FunctionGroupStates[state.fg_name] = state.fg_newState;
                log->update_logger(functions, this->FunctionGroupStates);
            }
            else
            {
                printf("%s not prepared correctly\n", fg.c_str());
                out.AppError = ara::sm::SM_ApplicationError::kPrepareFailed;
                log->update_logger(functions, this->FunctionGroupStates);
                break;
            }
        }
    }
    else
    {
        printf("StartUpdateSession must be called before\n");
        out.AppError = ara::sm::SM_ApplicationError::kRejected;
        log->update_logger(functions, this->FunctionGroupStates);
    }
    promise.set_value(out);

    return promise.get_future();
}
std::future<UpdateRequest::UpdateRequestSkeleton::VerifyUpdateOutput> UpdateRequestImpl::VerifyUpdate(FunctionGroupList FunctionGroups)
{
    std::cout << "Verifying Update" << std::endl;
    sm_functions functions = {.sm_StartUpdateSession = (0), .sm_StopUpdateSession = (0), .sm_PrepareUpdate = (0), .sm_VerifyUpdate = (1)};

    bool success;
    std::promise<UpdateRequestSkeleton::VerifyUpdateOutput> promise;
    UpdateRequestSkeleton::VerifyUpdateOutput out;
    if (this->FunctionGroupStates["machineFG"] == "update")
    {
        for (auto fg : FunctionGroups)
        {
            if (this->FunctionGroupStates[fg] != "off")
            {
                printf("PrepareUpdate must be called before\n");
                out.AppError = ara::sm::SM_ApplicationError::kRejected;
                break;
            }
            FunctionGroupState state = FunctionGroupState::Preconstruct(fg, "verify");
            success = client->setState(state);
            if (success)
            {
                std::cout << state.fg_name << " now is in state " << state.fg_newState << std::endl;
                out.AppError = ara::sm::SM_ApplicationError::kVerified;
                this->FunctionGroupStates[state.fg_name] = state.fg_newState;
                log->update_logger(functions, this->FunctionGroupStates);
            }
            else
            {
                printf("%s in not verifiyed correctly\n", fg.c_str());
                out.AppError = ara::sm::SM_ApplicationError::kVerifyFailed;
                log->update_logger(functions, this->FunctionGroupStates);
                break;
            }
        }
    }
    else
    {
        printf("StartUpdateSession must be called before\n");
        out.AppError = ara::sm::SM_ApplicationError::kRejected;
        log->update_logger(functions, this->FunctionGroupStates);
    }
    promise.set_value(out);

    return promise.get_future();
}
void UpdateRequestImpl::run_cluster(int cluster)
{
    if (cluster == (int)clusters::OTA)
    {
        printf("ota cluster received\n");
        FunctionGroupState s = FunctionGroupState::Preconstruct("fn2", "run");
        client->setState(s);
        FunctionGroupStates[s.fg_name] = s.fg_newState;
        log->update_logger({0, 0, 0, 0},FunctionGroupStates);
    }
    if (cluster == (int) clusters::UCM)
    {
        std::cout<<"ucm cluster received";
        FunctionGroupState s = FunctionGroupState::Preconstruct("machineFG", "running");
        //client->setState(s);
        client->setState(FunctionGroupState::Preconstruct("machineFG", "running"));
        //sleep(6);
        FunctionGroupStates[s.fg_name] = s.fg_newState;
        log->update_logger({0, 0, 0, 0},FunctionGroupStates);
    }
}
