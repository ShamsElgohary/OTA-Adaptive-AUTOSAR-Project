#ifndef SM_H
#define SM_H
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
#include <stdexcept>
#include <map>
#include <future>
#include "../exec/include/state_client.hpp"
#include "UpdateRequestSkeleton.hpp"
#include "types.hpp"
#include "sm_logger.hpp"
using namespace ara::exec;
namespace ara
{
    namespace sm
    {
        class UpdateRequestImpl : public ara::sm::skeleton::UpdateRequestSkeleton
        {
        public:
            UpdateRequestImpl(ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::sm::skeleton::UpdateRequestSkeleton(I_id, mode)
            {
                //freopen("sm.txt", "w+", stdout); // write cout and printf to txt file
                FunctionGroupStates = {};
            }
            sm_logger *log {new sm_logger(8088)};
            std::future<StartUpdateSessionOutput> StartUpdateSession();
            std::future<PrepareUpdateOutput> PrepareUpdate(FunctionGroupList FunctionGroups);
            std::future<VerifyUpdateOutput> VerifyUpdate(FunctionGroupList FunctionGroups);
            void StopUpdateSession();
            void run_cluster(int cluster);

        private:
            StateClient client;
            std::map<Functiongroup, std::string> FunctionGroupStates;
            friend class sm_logger;
            enum clusters
            {
                OTA,
                UCM
            };
        };
    }
}
#endif