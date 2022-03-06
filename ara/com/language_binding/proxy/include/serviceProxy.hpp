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
                    uint16_t Service_Interface_ID;
                    class HandleType 
                    {
                        private:
                        ara::com::InstanceIdentifier ID;
                        std::shared_ptr <ara::com::NetworkBase> network_binding ;

                        inline bool operator==(const HandleType &other) const;
                        const ara::com::InstanceIdentifier &GetInstanceId() const;
                    };
                    static ara::com::ServiceHandleContainer<ProxyBase::HandleType>FindService();

                    explicit ProxyBase (HandleType &handle);

                    ProxyBase (ProxyBase  &other) = delete;

                    ProxyBase & operator=(const ProxyBase &other) = delete;

            };
        }
    }
}

#endif
