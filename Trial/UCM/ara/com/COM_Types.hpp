#ifndef _ARA_COM_COM_TYPES_HPP_
#define _ARA_COM_COM_TYPES_HPP_

#include <stdint.h>
#include <memory>

namespace ara
{
    namespace com
    {
        enum class MethodCallProcessingMode { kPoll, kEvent, kEventSingleThread };


        /* INSTANCE IDENTIFIER */
        class InstanceIdentifier
        {
            public:


            private:
        }


        /* USED TO FIND HANDLER */
        template <typename H>
        class FindServiceHandler
        {
            public:


            private:

        };


        class InstanceIdentifierContainer
        {
            public:

            private:

        };

        class FindServiceHandle 
        {
            public:

            private:

        };

        template <typename H>
        class ServiceHandleContainer
        {
            public:

            private:
        };


        class NetworkBase
        {
            public:
                template <typename T, typename... args>
                ara::core::Future <T> SendRequest(uint16_t ID, args... Args);
        };


        class SubscriptionState
        {

        };

    }
}

#endif
