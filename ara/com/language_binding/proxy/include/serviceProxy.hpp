#pragma once
#include "../../../include/types.hpp"
#include "../../../network_binding/NetworkBinding.hpp"
#include <fstream>
#include "../../../../../utility/jsoncpp/header/json.h"
#include "../../../../ara/iam/lib/include/IAMGrantQuery.hpp"
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
                    ara::com::serviceIdentifierType serviceID;
                    ara::com::InstanceIdentifier InstanceID;

                public:
                    std::shared_ptr<ara::com::NetworkBindingBase> ptr2bindingProtocol;
                    const ara::com::InstanceIdentifier &GetInstanceId() const;
                    HandleType() = default;
                    HandleType(const HandleType &) = default;
                    HandleType &operator=(const HandleType &) = default;
                    HandleType(HandleType &&) = default;
                    HandleType &operator=(HandleType &&) = default;
                    ~HandleType() noexcept = default;
                    friend class ProxyBase;
                };
                static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(string path, int serviceID);
                static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(string path, int serviceID, ara::com::InstanceIdentifier instanceId);
                ProxyBase(HandleType handle);
                ProxyBase(ProxyBase &other) = delete;
                ProxyBase &operator=(const ProxyBase &other) = delete;
                HandleType ProxyHandle;
                static string parse(string x, int service_id, int instance_id);
            };
        }
    }
}
