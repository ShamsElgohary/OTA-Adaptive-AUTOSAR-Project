#ifndef _ARA_COM_INTERNAL_PROXY_HPP_
#define  _ARA_COM_INTERNAL_PROXY_HPP_

#include "COM_Types.hpp"
#include "CORE_Types.hpp"

/* GENERIC */

namespace ara
{
    namespace com
    {
        namespace Proxy
        {
            class ProxyBase {

                public:
                    class HandleType 
                    {
                        private:
                        ara::com::InstanceIdentifier ID;
                        std::shared_ptr <NetworkBase> network_binding ;

                        inline bool operator==(const HandleType &other) const;
                        const ara::com::InstanceIdentifier &GetInstanceId() const;
                    };
                    static ara::com::ServiceHandleContainer<Proxy::HandleType>FindService();

                    explicit Proxy(HandleType &handle);

                    Proxy(Proxy &other) = delete;

                    Proxy& operator=(const Proxy &other) = delete;

            };
        }
    }
}

#endif
