#pragma once
#include "../../../include/types.hpp"
#include "../../../network_binding/NetworkBinding.hpp"
#include<fstream>
#include <jsoncpp/json/json.h>
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
                    HandleType()=default;
                    HandleType(const HandleType &)=default;
                    HandleType &operator=(const HandleType &)=default;
                    HandleType(HandleType &&)=default;
                    HandleType &operator=(HandleType &&)=default;
                    ~HandleType() noexcept =default;
                    friend class ProxyBase;
                };
                static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(int serviceID);
                static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(int serviceID, ara::com::InstanceIdentifier instanceId);
                ProxyBase(HandleType handle);
                ProxyBase(ProxyBase &other) = delete;
                ProxyBase &operator=(const ProxyBase &other) = delete;
                HandleType ProxyHandle;
            };
        }
    }
}

