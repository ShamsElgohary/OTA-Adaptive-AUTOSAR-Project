#include "Internal/UpdateRequestProxy.hpp"
#include "memory"
using namespace ara::com::proxy;
int main()
{
    // some code to acquire the handle 
    std::unique_ptr<UpdateRequestProxy>proxy;
    auto handles=UpdateRequestProxy::FindService();
    if(!handles.empty())
    {
        proxy=std::make_unique<UpdateRequestProxy>(handles[0]);
    }
    //call function
    std::future<methods::StartUpdateSessionOutput>StartUpdateSession_Call_Future;
    StartUpdateSession_Call_Future=proxy->StartUpdateSession();
    // now we do a blocking get() which will return in case of result has set
    methods::StartUpdateSessionOutput StartUpdateSession_Call_Output=StartUpdateSession_Call_Future.get();
    return 0;
}
