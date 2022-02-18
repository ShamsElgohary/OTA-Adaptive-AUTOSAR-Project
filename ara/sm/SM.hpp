#ifndef SM_H
#define SM_H
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
#include <stdexcept>
namespace ara
{
    namespace sm
    {
        using Functiongroup = std::string;
        using FunctionGroupList=std::vector<Functiongroup>;
        enum class SM_ApplicationError : uint8_t
                {
                    kRejected = 5U,
                    kVerifyFailed = 6U,
                    kPrepareFailed = 7U,
                    kRollbackFailed = 8U,
                };
        class UpdateRequest
        {
        public:
            uint8_t StartUpdateSession();
            uint8_t PrepareUpdate(FunctionGroupList FunctionGroups); 
            uint8_t VerifyUpdate(FunctionGroupList FunctionGroups);  
            void StopUpdateSession();
        };
    }
}
#endif