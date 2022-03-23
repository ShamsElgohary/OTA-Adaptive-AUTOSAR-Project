#include "TestSkeleton.hpp"

int main ()
{
    Imp Service(1, ara::com::MethodCallProcessingMode::kEvent);
    Service.OfferService();
    return 0;
}