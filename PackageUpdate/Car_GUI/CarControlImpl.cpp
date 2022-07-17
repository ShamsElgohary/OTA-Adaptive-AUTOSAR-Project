#include "CarControlImpl.hpp"
#include "../../SecureOTA_System_Configurations/GeneratedFiles/CarControlSkeleton.hpp"


CarControlImpl::CarControlImpl(ara::com::InstanceIdentifier I_id):CarControlSkeleton(I_id)
{

}
std::future<carcontrol::CarControlSkeleton::ProcessCarActionOutput> CarControlImpl::ProcessCarAction(CarActionType Action)
{
    std::promise<ProcessCarActionOutput> promise;
    ProcessCarActionOutput out;
    promise.set_value(out);
    return promise.get_future();

}