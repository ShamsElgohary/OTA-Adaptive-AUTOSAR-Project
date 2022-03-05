#include "Internal/UpdateRequestProxy.hpp"
using namespace ara::com::proxy;
int main()
{
    // some code to acquire the handle 
    //...
    ara::com::internal::Proxy::HandleType handle;
    UpdateRequestProxy proxy{handle};
    //call function
    std::future<methods::StartUpdateSessionOutput>StartUpdateSession_Call_Future;
    StartUpdateSession_Call_Future=proxy.StartUpdateSession();
    // now we do a blocking get() which will return in case of result has set
    methods::StartUpdateSessionOutput StartUpdateSession_Call_Output=StartUpdateSession_Call_Future.get();
    return 0;
}