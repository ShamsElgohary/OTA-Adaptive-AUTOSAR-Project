#pragma once

const std::string Cluster_Name = "sm";
namespace ara
{
    namespace sm
    {
        using Functiongroup = std::string;
        using FunctionGroupList = std::vector<Functiongroup>;
        enum class SM_ApplicationError : uint8_t
        {
            kPrepared = 1U,
            kVerified = 1U,
            kRejected = 5U,
            kVerifyFailed = 6U,
            kPrepareFailed = 7U,
            kRollbackFailed = 8U,
        };
    }
}