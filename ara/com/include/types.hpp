#pragma once 
#include <iostream>
using namespace std ;

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

        class InstanceIdentifier
        {
            public:
            explicit InstanceIdentifier(const string_view value);
            const string_view toString() const;
            bool operator==(const InstanceIdentifier& other) const;
            bool operator<(const InstanceIdentifier& other) const;
            InstanceIdentifier& operator=(const InstanceIdentifier& other);
        };

        template <typename T>
        using ServiceHandleContainer = std::vector<T>;

    }
}
