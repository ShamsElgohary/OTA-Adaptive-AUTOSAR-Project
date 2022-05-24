#include "../exec/include/execution_client.hpp"
#include <iostream>
#include "SM.hpp"
#include <unistd.h>
using namespace std;
using namespace ara::sm;
using namespace ara::exec;
void handle_sigTerm(int sig)
{
    ExecutionClient exec;
    exec.ReportExecutionStaste(ExecutionState::Kterminate);
    exit(1);
}
int main()
{
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handle_sigTerm;
    sigaction(SIGTERM, &sa, NULL);
    sleep(2);
    ExecutionClient client{};
    client.ReportExecutionStaste(ExecutionState::Krunning);
    
    sm_logger logger(8088);

    StateClient x;
    x.setState(FunctionGroupState::Preconstruct("fn1", "idle"));
    sleep(6);
    x.setState(FunctionGroupState::Preconstruct("fn1", "play"));
    sleep(6);
    x.setState(FunctionGroupState::Preconstruct("fn1", "terminate"));
    sleep(1);
    x.setState(FunctionGroupState::Preconstruct("machineFG", "running"));
    sleep(6);
    x.setState(FunctionGroupState::Preconstruct("fn2", "idle"));
    //UpdateRequestImpl updaterequest(1, ara::com::MethodCallProcessingMode::kEvent);
    //updaterequest.log->gui_receive();
    /********Test*************/

    FunctionGroupList function_groups = {"fn1", "fn2"};
    // std::future<skeleton::UpdateRequestSkeleton::StartUpdateSessionOutput> out=updaterequest.StartUpdateSession();
    // //std::cout<<"return is "<<static_cast<unsigned>(out.get().AppError)<<std::endl;
    // std::future<skeleton::UpdateRequestSkeleton::PrepareUpdateOutput> out1=updaterequest.PrepareUpdate(function_groups);
    // //std::cout<<"return is "<<static_cast<unsigned>(out1.get().AppError)<<std::endl;
    // std::future<skeleton::UpdateRequestSkeleton::VerifyUpdateOutput> out2=updaterequest.VerifyUpdate(function_groups);
    // //std::cout<<"return is "<<static_cast<unsigned>(out2.get().AppError)<<std::endl;
    //updaterequest.StopUpdateSession();

    /******Offer_Service*************/
    /*std::thread([&updaterequest]()
                {
            cout<<"Sm offering service"<<endl;

        updaterequest.OfferService(); })
        .detach();*/
    //x.setState(FunctionGroupState::Preconstruct("fn2", "idle"));
     UpdateRequestImpl updaterequest(&logger,1, ara::com::MethodCallProcessingMode::kEvent);
     cout<<"Sm offering service"<<endl;
     updaterequest.OfferService();
     //sleep(1000);
     sleep(1000);
}