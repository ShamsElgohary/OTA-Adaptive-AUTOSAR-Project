#pragma once
#include <iostream>
#include <thread>
#include <utility>
#include <map>
#include "../../../include/types.hpp"
#include "../../../network_binding/NetworkBinding.hpp"
#include "../../../../../utility/jsoncpp/header/json.h"
#include "../../../../iam/lib/include/IAMGrantQuery.hpp"
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
                bool grant_result;
                void serve();
                ara::com::serviceIdentifierType serviceID;
                ara::com::InstanceIdentifier instanceID;
                ara::com::MethodCallProcessingMode mode;
                std::shared_ptr<ara::com::NetworkBindingBase> ptr2bindingProtocol;
                uint32_t port;
                string ip;
                virtual void handleMethod() = 0;
                skeletonBase(string path, ara::com::serviceIdentifierType serviceID, ara::com::InstanceIdentifier instanceId, std::string C_name ,ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent);
                skeletonBase(const skeletonBase &other) = delete;
                skeletonBase &operator=(const skeletonBase &other) = delete;
                skeletonBase() = delete;
                ~skeletonBase(){};
                void OfferService();
                void StopOfferService();
                string C_Name; 
            };
        }
    }
}