#ifndef SM_H
#define SM_H
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
#include <stdexcept>
#include <map>
#include <future>
#include "UpdateRequestSkeleton.hpp"
#include "types.hpp"
namespace ara
{
    namespace sm
    {
        class UpdateRequestImpl : public ara::sm::skeleton::UpdateRequestSkeleton
        {
        public:
            UpdateRequestImpl(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::sm::skeleton::UpdateRequestSkeleton(I_id, mode)
            {
            }
            std::future<StartUpdateSessionOutput> StartUpdateSession();
            std::future<PrepareUpdateOutput> PrepareUpdate(FunctionGroupList FunctionGroups);
            std::future<VerifyUpdateOutput> VerifyUpdate(FunctionGroupList FunctionGroups);
            void StopUpdateSession();

        private:
            std::map<Functiongroup, std::string> FunctionGroupStates;
        };
    }
}
#endif