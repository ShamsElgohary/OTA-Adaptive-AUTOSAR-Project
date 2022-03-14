#ifndef _ARA_COM_INTERNAL_PROXY_HPP_
#define _ARA_COM_INTERNAL_PROXY_HPP_

#include "../../../include/types.hpp"
#include "../../../network_binding/network_binding_base.hpp"
#include "handleType.hpp"

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
                static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService();

                /* EXTRA */
                static ara::com::ServiceHandleContainer<ProxyBase::HandleType>
                FindService(ara::com::InstanceIdentifier instanceId);

                /* EXTRA (FindServiceHandler & FindServiceHandle are not Implemented) */
                static ara::com::FindServiceHandle StartFindService(
                    ara::com::FindServiceHandler<ProxyBase::HandleType> handler);

                /* EXTRA  (FindServiceHandler & FindServiceHandle are not Implemented) */
                static ara::com::FindServiceHandle StartFindService(
                    ara::com::FindServiceHandler<ProxyBase::HandleType> handler,
                    ara::com::InstanceIdentifier instanceId);

                /* EXTRA (FindServiceHandle is not Implemented) */
                static void StopFindService(ara::com::FindServiceHandle handle);

                /* IMPLEMENTATION IN GENERATION */
                ProxyBase(HandleType handle);

                /* DELETE COPY CONSTRUCTORS */
                ProxyBase(ProxyBase &other) = delete;
                ProxyBase &operator=(const ProxyBase &other) = delete;

            protected:
                //HandleType Handle;
                HandleType& Delegate;
                ara::com::serviceIdentifierType serviceID;
            };
        }
    }
}

#endif
