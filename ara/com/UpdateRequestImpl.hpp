#include <future>
#include "UpdateRequestSkeleton.hpp"
using namespace ara::com::skeleton;
class UpdateRequestImpl : public UpdateRequestSkeleton
{
public:
   std::future<StartUpdateSessionOutput> StartUpdateSession();
   void StopUpdateSession();
   std::future<VerifyUpdateOutput> VerifyUpdate(ara::sm::FunctionGroupList Functiongroups);
   std::future<PrepareUpdateOutput> PrepareUpdate(ara::sm::FunctionGroupList Functiongroups);

private:
   // internal implementation of functions using network binding
   ara::com::skeleton::UpdateRequestSkeleton::StartUpdateSessionOutput doStartUpdateSessionInternal();
   void doStopUpdateSessionInternal();
   ara::com::skeleton::UpdateRequestSkeleton::PrepareUpdateOutput doPrepareUpdateInternal(ara::sm::FunctionGroupList functiongroups);
   ara::com::skeleton::UpdateRequestSkeleton::VerifyUpdateOutput doVerifyUpdateInternal(ara::sm::FunctionGroupList functiongroups);
};