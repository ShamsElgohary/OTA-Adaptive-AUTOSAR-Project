#pragma once
#include <stdint.h>
#include <memory>
#include <vector>
namespace ara
{
    namespace com
    {
        enum class MethodCallProcessingMode
        {
            kPoll,
            kEvent,
            kEventSingleThread
        };
        using serviceIdentifierType = uint32_t;
        using InstanceIdentifier = uint32_t;
        class InstanceIdentifierContainer : public std::vector<InstanceIdentifier>
        {
        };
        template <typename H>
        class ServiceHandleContainer : public std::vector<H>
        {
        };
    }
}