#pragma once
#include <iostream>
#include <thread>
#include <utility>
#include <map>
#include "../../../include/types.hpp"
#include "../../../network_binding/NetworkBinding.hpp"
#include "../../../../../jsoncpp/header/json.h"

using namespace std;

namespace ara
{
    namespace com
    {
        namespace skeleton
        {
            class skeletonBase
            {
            public:
                ara::com::serviceIdentifierType serviceID;
                ara::com::InstanceIdentifier instanceID;
                ara::com::MethodCallProcessingMode mode;
                std::shared_ptr<ara::com::NetworkBindingBase> ptr2bindingProtocol;
                uint32_t port;
                string ip;
                void serve();
                virtual void handleMethod(int methodID){}
                skeletonBase(ara::com::serviceIdentifierType serviceID, ara::com::InstanceIdentifier instanceId, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent);
                skeletonBase(const skeletonBase &other) = delete;
                skeletonBase &operator=(const skeletonBase &other) = delete;
                skeletonBase()=delete;
                ~skeletonBase()=default;
                void OfferService();
                void StopOfferService();
            };
        }
    }
}