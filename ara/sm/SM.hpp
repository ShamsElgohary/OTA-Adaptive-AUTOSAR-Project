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
        class UpdateRequestImpl : public ara::sm::UpdateRequest::UpdateRequestSkeleton
        {
        public:
            UpdateRequestImpl(sm_logger*logger,ara::com::InstanceIdentifier I_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::sm::UpdateRequest::UpdateRequestSkeleton(I_id, mode)
            {
                //freopen("sm.txt", "w+", stdout); // write cout and printf to txt file
                FunctionGroupStates = {};
                this->log=logger;
                client=new StateClient();

            }
            sm_logger *log;
            std::future<StartUpdateSessionOutput> StartUpdateSession();
            std::future<PrepareUpdateOutput> PrepareUpdate(FunctionGroupList FunctionGroups);
            std::future<VerifyUpdateOutput> VerifyUpdate(FunctionGroupList FunctionGroups);
            void StopUpdateSession();
            void run_cluster(int cluster);
            UpdateRequestImpl(const UpdateRequestImpl &other)=default;


            private:
            StateClient* client;
            static std::map<Functiongroup, std::string> FunctionGroupStates;
            friend class sm_logger;
            enum clusters
            {
                OTA=1,
                UCM=2
            };
        };
    }
}
#endif