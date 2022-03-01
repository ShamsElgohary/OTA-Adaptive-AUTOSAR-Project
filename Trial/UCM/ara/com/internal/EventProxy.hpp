#ifndef _ARA_COM_INTERNAL_EVENTPROXY_HPP_
#define  _ARA_COM_INTERNAL_EVENTPROXY_HPP_

#include "COM_Types.hpp"
#include "CORE_Types.hpp"

/* GENERIC */

namespace ara
{
    namespace com
    {
        namespace internal
        {
            class EventProxy {

                public:

                /* MUST */
                void Subscribe(size_t maxSampleCount);

                ara::com::SubscriptionState GetSubscriptionState() const;

                /* MUST */
                void Unsubscribe();

                ara::core::Result<size_t> GetFreeSampleCount() const noexcept;


                void SetReceiveHandler(ara::com::EventReceiveHandler handler);

                void UnsetReceiveHandler();

                void SetSubscriptionStateChangeHandler(ara::com::SubscriptionStateChangeHandler handler);

                void UnsetSubscriptionStateChangeHandler();

                /* MUST */
                template <typename F>
                ara::core::Result<size_t> GetNewSamples(F&& f,
                size_t maxNumberOfSamples = std::numeric_limits<size_t>::max());


            };
        }
    }
}

#endif