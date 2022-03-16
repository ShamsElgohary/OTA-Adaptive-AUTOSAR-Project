#ifndef _ARA_COM_COM_TYPES_HPP_
#define _ARA_COM_COM_TYPES_HPP_

#include <stdint.h>
#include <memory>
#include <vector>

namespace ara
{
    namespace com
    {
        enum class MethodCallProcessingMode { kPoll, kEvent, kEventSingleThread };
        
        /* SERVICE IDENTIFIER */
        using serviceIdentifierType = std::string; //should be int? Yes

        /* SERVICE INSTANCE IDENTIFIER */
        using InstanceIdentifier = uint32_t; //should be string? No

        /* SERVICE INSTANCE IDENTIFIER CONTAINER */
        class InstanceIdentifierContainer : public std::vector<InstanceIdentifier>
        {};

        /* SERVICE HANDLE CONTAINER */
        template <typename H>
        class ServiceHandleContainer : public std::vector<H>
        {};


        enum class BindingProtocol
        {
            someip,
            dds
        };




        /* NOT IMPLEMENTED */
        class FindServiceHandle 
        {
            public:

            private:

        };


        /* NOT IMPLEMENTED */
        template <typename H>
        class FindServiceHandler
        {
            public:


            private:

        };

        /* NOT IMPLEMENTED */
        class SubscriptionState
        {

        };

    }
}

#endif
