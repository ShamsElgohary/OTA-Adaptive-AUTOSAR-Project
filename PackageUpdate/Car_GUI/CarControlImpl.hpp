#include <iostream>
#include <future>
#include "../../SecureOTA_System_Configurations/GeneratedFiles/CarControlSkeleton.hpp"
using namespace std;

class CarControlImpl : public carcontrol::CarControlSkeleton
{
public:
    CarControlImpl(ara::com::InstanceIdentifier I_id);
    std::future<ProcessCarActionOutput> ProcessCarAction(CarActionType Action);
};
