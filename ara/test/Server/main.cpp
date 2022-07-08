#include "TestSkeleton.hpp"
#include "execution_client.hpp"

int main ()
{
    ara::exec::ExecutionClient exec;
    exec.ReportExecutionStaste(ara::exec::ExecutionState::Krunning);
    Imp Service(1, ara::com::MethodCallProcessingMode::kEvent);
    Service.OfferService();
    return 0;
}