#include "ara/com/types.hpp"
#include "ara/sm/SM.hpp"
#include "Proxy.hpp"
#include "MethodProxy.hpp"
#include<future>
namespace ara
{
    namespace com
    {
        namespace proxy
        {
            namespace methods
            { 
                struct StartUpdateSessionOutput
                    {
                        uint8_t success;
                    };
                class StartUpdateSession : public ara::com::internal::MethodProxy<StartUpdateSessionOutput>
                {
                    public:
                    StartUpdateSession();
                    StartUpdateSession(ara::com::internal::Proxy::HandleType& handle):MethodProxy(handle,1)
                    {}
                };
                class StopUpdateSession : public internal::MethodOneWayProxy
                {
                    public:
                    StopUpdateSession();
                    StopUpdateSession(ara::com::internal::Proxy::HandleType&handle):MethodOneWayProxy(handle,2)
                    {}
        
                };
                struct PrepareUpdateOutput
                    {
                        uint8_t success;
                    };
                class PrepareUpdate : public ara::com::internal::MethodProxy<PrepareUpdateOutput>
                {
                    public:
                    PrepareUpdate();
                    PrepareUpdate(ara::com::internal::Proxy::HandleType& handle):MethodProxy(handle,3)
                    {}
                };
                struct VerifyUpdateOutput
                    {
                        uint8_t success;
                    };
                class VerifyUpdate :public ara::com::internal::MethodProxy<VerifyUpdateOutput>
                {
                    public:
                    VerifyUpdate();
                    VerifyUpdate(ara::com::internal::Proxy::HandleType& handle):MethodProxy(handle,4)
                    {}
                };
            }
            class UpdateRequestProxy : public internal::Proxy
            {
            public:
                methods::StartUpdateSession StartUpdateSession;
                methods::StopUpdateSession StopUpdateSession;
                methods::PrepareUpdate PrepareUpdate;
                methods::VerifyUpdate VerifyUpdate;
                /* EXPLICIT CONSTRUCTOR */
                explicit UpdateRequestProxy(HandleType &handle):Proxy(handle)
                {
                StartUpdateSession(handle);
                StopUpdateSession(handle);
                PrepareUpdate(handle);
                VerifyUpdate(handle);
                }
                

            private:
            };
        }
    }
}
