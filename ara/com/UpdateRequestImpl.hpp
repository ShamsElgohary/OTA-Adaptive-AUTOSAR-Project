#include <future>
#include <UpdateRequestSkeleton.hpp>
using namespace ara::com;
class UpdateRequestImpl : public UpdateRequestSkeleton
{
public:
   // implementation of virtual functions using private functions and future
private:
   // internal implementation of functions using network binding
   com::skeleton::UpdateRequestSkeleton::StartUpdateSessionOutput doStartUpdateSessionInternal();
   void doStopUpdateSessionInternal();
   com::skeleton::UpdateRequestSkeleton::PrepareUpdateOutput doPrepareUpdateInternal(ara::sm::FunctionGroupList functiongroups);
   com::skeleton::UpdateRequestSkeleton::VerifyUpdateOutput doVerifyUpdateInternal(ara::sm::FunctionGroupList functiongroups);
};