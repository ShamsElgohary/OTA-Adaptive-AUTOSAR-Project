#pragma once
#include <iostream>
#include <vector>
#include "GUI_Report.hpp"


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
        using serviceIdentifierType = uint16_t;
        using InstanceIdentifier = uint16_t;
        class InstanceIdentifierContainer : public std::vector<InstanceIdentifier>
        {
        };
        template <typename H>
        class ServiceHandleContainer : public std::vector<H>
        {
        };
    }
}