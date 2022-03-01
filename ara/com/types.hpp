#pragma once
#include <iostream>
#include <functional>
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
            explicit InstanceIdentifier(std::string value);
            std::string ToString() const;
            bool operator==(const InstanceIdentifier &other) const;
            bool operator<(const InstanceIdentifier &other) const;
            InstanceIdentifier &operator=(const InstanceIdentifier &other);

        private:
        };
        struct FindServiceHandle
        {
            uint16_t serviceIdentifier;
            uint16_t instanceIdentifier;
            uint32_t uid;

            bool operator==(const FindServiceHandle &other) const;
            bool operator<(const FindServiceHandle &other) const;
            FindServiceHandle &operator=(const FindServiceHandle &other) const;
        };

        template <typename T>
        using ServiceHandleContainer = std::vector<T>;

        template <typename T>
        using FindServiceHandler = std::function<void(ServiceHandleContainer<T>, FindServiceHandle)>;
    }
}
