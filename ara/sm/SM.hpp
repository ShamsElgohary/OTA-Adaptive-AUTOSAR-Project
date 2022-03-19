#ifndef SM_H
#define SM_H
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
#include <stdexcept>
#include <map>
#include "UpdateRequestSkeleton.hpp"
#include "types.hpp"

namespace ara
{
    namespace sm
    {
        class UpdateRequestImpl : public ara::sm::UpdateRequestSkeleton
        {
        public:
            uint8_t StartUpdateSession();
            uint8_t PrepareUpdate(FunctionGroupList FunctionGroups);
            uint8_t VerifyUpdate(FunctionGroupList FunctionGroups);
            void StopUpdateSession();

        private:
            std::map<Functiongroup, std::string> FunctionGroupStates;
        };
    }
}
#endif