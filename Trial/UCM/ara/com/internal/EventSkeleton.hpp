#ifndef _ARA_COM_INTERNAL_EVENTSKELETON_HPP_
#define  _ARA_COM_INTERNAL_EVENTSKELETON_HPP_

#include "COM_Types.hpp"
#include "CORE_Types.hpp"

namespace ara
{
    namespace com
    {
        namespace internal
        {
            template <typename T>
            class EventSkeleton {

                public:
                void Send(const T &data);

                ara::com::SampleAllocateePtr<T> Allocate();

                /* MUST */
                void Send(ara::com::SampleAllocateePtr<T> data);


            };
        }
    }
}

#endif