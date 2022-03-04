#include "ara/com/types.hpp"
#include "ara/sm/SM.hpp"
#include<future>
namespace ara
{
    namespace com
    {
        namespace proxy
        {
            namespace methods
            {
                class StartUpdateSession
                {
                    struct output
                    {
                        uint8_t success;
                    };
                    std::future<output> operator()(void);
                };
                class StopUpdateSession
                {
                    struct output
                    {
                    };
                    std::future<void> operator()(void);
                };
                class StartUpdateSession
                {
                    struct output
                    {
                        uint8_t success;
                    };
                    std::future<output> operator()(const uint64_t size);
                };
                class PrepareUpdate
                {
                    struct output
                    {
                        uint8_t success;
                    };
                    std::future<output> operator()(ara::sm::FunctionGroupList functiongroups);
                };
                class VerifyUpdate
                {
                    struct output
                    {
                        uint8_t success;
                    };
                    std::future<output> operator()(ara::sm::FunctionGroupList functiongroups);
                };
            }
            class UpdateRequestProxy
            {
            public:
                /* IDENTIFIES THE SERVICE & CONTAINS NEEDED ADDRESSING INFORMATION (IN PROXY) */
                class HandleType
                {
                public:
                    bool operator==(const HandleType &other) const;
                    HandleType &operator=(const HandleType &other);
                    const ara::com::InstanceIdentifier &GetInstanceId() const;

                private:
                };

                static ara::com::FindServiceHandle StartFindService(
                    ara::com::FindServiceHandler<UpdateRequestProxy::HandleType> handler,
                    ara::com::InstanceIdentifier instanceId);

                /* METHOD TO STOP FINDING SERVICE REQUEST*/
                static void StopFindService(ara::com::FindServiceHandle handle);

                /*
                * Opposed to StartFindService(handler, instance) this version
                * is a "one-shot" find request, which is:
                * - synchronous, i.e. it returns after the find has been done
                and a result list of matching service instances is
                *
                available. (list may be empty, if no matching service
                *
                instances currently exist) */
                static ara::com::ServiceHandleContainer<UpdateRequestProxy::HandleType> FindService(ara::com::InstanceIdentifier instanceId);

                /* EXPLICIT CONSTRUCTOR */
                explicit UpdateRequestProxy(HandleType &handle);

                UpdateRequestProxy(UpdateRequestProxy &other) = delete;
                UpdateRequestProxy &operator=(const UpdateRequestProxy &other) = delete;

                /*methods*/
                methods::StartUpdateSession StartUpdateSession;
                methods::StopUpdateSession StopUpdateSession;
                methods::PrepareUpdate PrepareUpdate;
                methods::VerifyUpdate VerifyUpdate;

            private:
            };
        }
    }
}
