#ifndef _ARA_COM_INTERNAL_PROXY_HPP_
#define  _ARA_COM_INTERNAL_PROXY_HPP_

#include "../../../include/types.hpp"
#include "../../../network_binding/network_binding_base.hpp"

/* GENERIC */

namespace ara
{
    namespace com
    {
        namespace proxy
        {
            class ProxyBase {
                public:
                    class HandleType 
                    {
                        protected:
                            uint32_t VersionMajor;
                            uint32_t VersionMinor;

                            ara::com::InterfaceIdentifier InterfaceID;
                            ara::com::InstanceIdentifier InstanceID;

                            std::shared_ptr <ara::com::NetworkBase> network_binding ;

                        public:
                            inline bool operator==(const HandleType &other) const;

                            const ara::com::InstanceIdentifier &GetInstanceId() const
                            {
                                return InstanceID;
                            }

                            /* COPY CONSTRUCTORS */
                            HandleType(const HandleType&);
                            HandleType& operator=(const HandleType&);

                            /* MOVE CONSTRUCTORS */
                            HandleType(HandleType &&);
                            HandleType& operator=(HandleType &&);

                            ~HandleType() noexcept;

                            friend class proxy::ProxyBase;
                            
                    };

                    /* MUST */
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService();

                    /* EXTRA */
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType>
                    FindService(ara::com::InstanceIdentifier instanceId);

                    /* EXTRA (InstanceSpecifier is not Implemented) */
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType>
                    FindService(ara::core::InstanceSpecifier instanceSpec);

                    /* EXTRA (FindServiceHandler & FindServiceHandle are not Implemented) */
                    static ara::com::FindServiceHandle StartFindService(
                    ara::com::FindServiceHandler<ProxyBase::HandleType> handler);

                    /* EXTRA  (FindServiceHandler & FindServiceHandle are not Implemented) */
                    static ara::com::FindServiceHandle StartFindService(
                    ara::com::FindServiceHandler<ProxyBase::HandleType> handler,
                    ara::com::InstanceIdentifier instanceId);

                    /* EXTRA (InstanceSpecifier & FindServiceHandler & FindServiceHandle are not Implemented) */
                    static ara::com::FindServiceHandle StartFindService(
                    ara::com::FindServiceHandler<ProxyBase::HandleType> handler,
                    ara::core::InstanceSpecifier instanceSpec);

                    /* EXTRA (FindServiceHandle is not Implemented) */
                    static void StopFindService(ara::com::FindServiceHandle handle);

                    /* IMPLEMENTATION IN GENERATION */
                    explicit ProxyBase (HandleType &handle);

                    /* DELETE COPY CONSTRUCTORS */
                    ProxyBase (ProxyBase  &other) = delete;
                    ProxyBase & operator=(const ProxyBase &other) = delete;

                protected:
                    ProxyBase::HandleType Handle;
                    ara::com::serviceIdentifierType;
            };
        }
    }
}

#endif
