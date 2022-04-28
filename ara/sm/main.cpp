#include "../exec/include/execution_client.hpp"
#include <iostream>
#include "SM.hpp"
using namespace std;
using namespace ara::sm;
using namespace ara::exec;

int main()
{
    printf("SM started to work\n");
    /********Report to EM*****/
    // ExecutionClient client{};
    // client.ReportExecutionStaste(ExecutionState::Krunning);
    /********Test*************/
    UpdateRequestImpl updaterequest(1, ara::com::MethodCallProcessingMode::kEvent);
    FunctionGroupList function_groups = {"fg1", "fg2"};
    /*********Test*******************/
    std::future<skeleton::UpdateRequestSkeleton::StartUpdateSessionOutput> out=updaterequest.StartUpdateSession();
    std::cout<<std::endl<<unsigned(out.get().AppError);
    updaterequest.PrepareUpdate(function_groups);
    updaterequest.VerifyUpdate(function_groups);
    updaterequest.StopUpdateSession();
    /******Offer_Service*************/
    updaterequest.OfferService();
}