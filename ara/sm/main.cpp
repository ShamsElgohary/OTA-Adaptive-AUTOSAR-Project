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
    sm_logger logger(8088);
    ExecutionClient client{};
    client.ReportExecutionStaste(ExecutionState::Krunning);
    StateClient x;
    // x.setState(FunctionGroupState::Preconstruct("fn1", "idle"));
    // sleep(1);
    // x.setState(FunctionGroupState::Preconstruct("fn1", "play"));
    // sleep(4);
    // x.setState(FunctionGroupState::Preconstruct("fn1", "terminate"));
    x.setState(FunctionGroupState::Preconstruct("machineFG", "running"));
   
    /********Test*************/
    UpdateRequestImpl updaterequest(&logger, 1, ara::com::MethodCallProcessingMode::kEvent);
    updaterequest.log->gui_receive(&updaterequest);
    /******Offer_Service*************/
    std::thread([&updaterequest]()
                {
            cout<<"Sm offering service"<<endl;
            updaterequest.OfferService(); })
        .join();
    x.setState(FunctionGroupState::Preconstruct("fn1", "play"));
    //x.setState(FunctionGroupState::Preconstruct("fn2", "idle"));
}