#pragma once
#include "types.hpp"
#include "NetworkBinding.hpp"
namespace ara
{
    namespace com
    {
        namespace proxy
        {
            class ProxyBase
            {
            public:
                class HandleType
                {
                public:
                    ara::com::InstanceIdentifier InstanceID;
                    std::shared_ptr<ara::com::NetworkBindingBase> ptr2bindingProtocol;
                    const ara::com::InstanceIdentifier &GetInstanceId() const;
                    HandleType();
                    HandleType(const HandleType &);
                    HandleType &operator=(const HandleType &);
                    HandleType(HandleType &&);
                    HandleType &operator=(HandleType &&);
                    ~HandleType() noexcept;
                    friend class ProxyBase;
                };
                static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(int serviceID);
                static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(int serviceID, ara::com::InstanceIdentifier instanceId);
                static ara::com::serviceIdentifierType serviceID;
                ProxyBase(HandleType handle);
                ProxyBase(ProxyBase &other) = delete;
                ProxyBase &operator=(const ProxyBase &other) = delete;
                HandleType ProxyHandle;
            };

        }
    }
}