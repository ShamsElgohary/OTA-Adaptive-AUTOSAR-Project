#ifndef _ARA_COM_INTERNAL_PROXY_HPP_
#define  _ARA_COM_INTERNAL_PROXY_HPP_

#include "types.hpp"
#include <memory>

/* GENERIC */

namespace ara
{
    namespace com
    {
        namespace internal
        {
            class Proxy {

                public:
                    class HandleType 
                    {
                        private:
                        ara::com::InstanceIdentifier ID;
                        std::shared_ptr <NetworkBase> network_binding ;
                        public:
                        HandleType();
                        inline bool operator==(const HandleType &other) const;
                        const ara::com::InstanceIdentifier &GetInstanceId() const;
                    };
                    static ara::com::FindServiceHandle StartFindService(
                    ara::com::FindServiceHandler<Proxy::HandleType> handler,
                    ara::com::InstanceIdentifier instanceId);

                   // static ara::com::FindServiceHandle StartFindService(
                   // ara::com::FindServiceHandler<Proxy::HandleType> handler,
                   // ara::core::InstanceSpecifier instanceSpec);

                    static ara::com::FindServiceHandle StartFindService(
                    ara::com::FindServiceHandler<Proxy::HandleType> handler);

                    static void StopFindService(ara::com::FindServiceHandle handle);
                    static ara::com::ServiceHandleContainer<Proxy::HandleType>
                    FindService(ara::com::InstanceIdentifier instanceId);

                   //static ara::com::ServiceHandleContainer<Proxy::HandleType>
                   //FindService(
                   //ara::core::InstanceSpecifier instanceSpec);

                    static ara::com::ServiceHandleContainer<Proxy::HandleType>FindService();

                    explicit Proxy(HandleType &handle);

                    Proxy(Proxy &other) = delete;

                    Proxy& operator=(const Proxy &other) = delete;

            };
        }
    }
}

#endif
