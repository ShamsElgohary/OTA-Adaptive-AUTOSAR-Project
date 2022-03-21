#ifndef _ARA_COM_INTERNAL_PROXY_HPP_
#define _ARA_COM_INTERNAL_PROXY_HPP_

#include "../../../include/types.hpp"
#include "handleType.hpp"
#include"../../../network_binding/NetworkBinding.hpp"

/* GENERIC */

namespace ara
{
    namespace com
    {
        namespace proxy
        {
            class ProxyBase
            {
            public:
                /* MUST */
                static ara::com::ServiceHandleContainer<ara::com::proxy::HandleType> FindService();

                /* EXTRA */
                static ara::com::ServiceHandleContainer<ara::com::proxy::HandleType>
                FindService(ara::com::InstanceIdentifier instanceId);

                /* EXTRA (FindServiceHandler & FindServiceHandle are not Implemented) */
                static ara::com::FindServiceHandle StartFindService(
                    ara::com::FindServiceHandler<ara::com::proxy::HandleType> handler);

                /* EXTRA  (FindServiceHandler & FindServiceHandle are not Implemented) */
                static ara::com::FindServiceHandle StartFindService(
                    ara::com::FindServiceHandler<ara::com::proxy::HandleType> handler,
                    ara::com::InstanceIdentifier instanceId);

                /* EXTRA (FindServiceHandle is not Implemented) */
                static void StopFindService(ara::com::FindServiceHandle handle);

                /* IMPLEMENTATION IN GENERATION */
                ProxyBase(HandleType handle);

                /* DELETE COPY CONSTRUCTORS */
                ProxyBase(ProxyBase &other) = delete;
                ProxyBase &operator=(const ProxyBase &other) = delete;
                
                HandleType ProxyHandle;
                ara::com::serviceIdentifierType serviceID;
            };
        }
    }
}

#endif
