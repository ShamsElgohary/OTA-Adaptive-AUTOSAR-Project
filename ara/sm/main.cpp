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
    ExecutionClient client{};
    client.ReportExecutionStaste(ExecutionState::Krunning);
    UpdateRequestImpl updaterequest(1, ara::com::MethodCallProcessingMode::kEvent);
    updaterequest.log->gui_receive();
    /********Test*************/
    
    // FunctionGroupList function_groups = {"fn1", "fn2"};
    // std::future<skeleton::UpdateRequestSkeleton::StartUpdateSessionOutput> out=updaterequest.StartUpdateSession();
    // //std::cout<<"return is "<<static_cast<unsigned>(out.get().AppError)<<std::endl;
    // std::future<skeleton::UpdateRequestSkeleton::PrepareUpdateOutput> out1=updaterequest.PrepareUpdate(function_groups);
    // //std::cout<<"return is "<<static_cast<unsigned>(out1.get().AppError)<<std::endl;
    // std::future<skeleton::UpdateRequestSkeleton::VerifyUpdateOutput> out2=updaterequest.VerifyUpdate(function_groups);
    // //std::cout<<"return is "<<static_cast<unsigned>(out2.get().AppError)<<std::endl;
    // updaterequest.StopUpdateSession();
    
    /******Offer_Service*************/
    
    updaterequest.OfferService();
}