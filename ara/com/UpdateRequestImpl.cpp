#include "UpdateRequestImpl.hpp"
class UpdateRequestImpl : public UpdateRequestSkeleton
{
public:
    std::future<StartUpdateSessionOutput> StartUpdateSession()
    {
        // calling synchronous internal StartUpdateSession function which delivers results
        std::promise<StartUpdateSessionOutput> promise;
        struct StartUpdateSessionOutput out = doStartUpdateSessionInternal();
        promise.set_value(out);
        // we return a future from an already set promise
        return promise.get_future();
    }
    void StopUpdateSession()
    {
        // one way fire and forget function (without future)
        doStopUpdateSessionInternal();
    }
    std::future<VerifyUpdateOutput> VerifyUpdate(ara::sm::FunctionGroupList Functiongroups)
    {
        // calling synchronous internal VerifyUpdate function which delivers results
        std::promise<VerifyUpdateOutput> promise;
        struct VerifyUpdateOutput out = doVerifyUpdateInternal(Functiongroups);
        promise.set_value(out);
        // we return a future from an already set promise
        return promise.get_future();
    }
    std::future<PrepareUpdateOutput> PrepareUpdate(ara::sm::FunctionGroupList Functiongroups)
    {
        // calling synchronous internal PrepareUpdateSession function which delivers results
        std::promise<PrepareUpdateOutput> promise;
        struct PrepareUpdateOutput out = doPrepareUpdateInternal(Functiongroups);
        promise.set_value(out);
        // we return a future from an already set promise
        return promise.get_future();
    }

private:
    // internal implementation of functions to get results using network binding
    ara::com::skeleton::UpdateRequestSkeleton::StartUpdateSessionOutput doStartUpdateSessionInternal();
    void doStopUpdateSessionInternal();
    ara::com::skeleton::UpdateRequestSkeleton::PrepareUpdateOutput doPrepareUpdateInternal(ara::sm::FunctionGroupList functiongroups);
    ara::com::skeleton::UpdateRequestSkeleton::VerifyUpdateOutput doVerifyUpdateInternal(ara::sm::FunctionGroupList functiongroups);
};