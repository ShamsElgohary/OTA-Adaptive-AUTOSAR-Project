#pragma once 
#include <iostream>
#include "serviceProxy.hpp"

using namespace std ;

namespace ara 
{
    namespace com
    {
        namespace proxy 
        {
            namespace event{

                template <typename T>
                class EventBase {
                    protected: 
                        ara::com::proxy::ProxyBase::HandleType Handle;
                        std::string name;
                        uint16_t ID;

                    public:
                        /* IMPLEMENTATION IN GENERATION */
                        EventBase(ProxyBase::HandleType& Handle);

                        /* MUST */
                        void Subscribe(size_t maxSampleCount);

                        /* EXTRA (SubscriptionState is not Implemented) */
                        ara::com::SubscriptionState GetSubscriptionState() const;

                        /* MUST */
                        void Unsubscribe();

                        /* EXTRA (Result is not Implemented yet) */
                        ara::core::Result<size_t> GetFreeSampleCount() const noexcept;

                        /* EXTRA (EventReceiveHandler is not Implemented) */
                        void SetReceiveHandler(ara::com::EventReceiveHandler handler);

                        /* EXTRA */
                        void UnsetReceiveHandler();

                        /* EXTRA (SubscriptionStateChangeHandler is not Implemented) */
                        void SetSubscriptionStateChangeHandler(ara::com::SubscriptionStateChangeHandler handler);

                        /* EXTRA */
                        void UnsetSubscriptionStateChangeHandler();

                        /* MUST (If there is an Easier way go ahead) */
                        template <typename F>
                        ara::core::Result<size_t> GetNewSamples(F&& f,
                        size_t maxNumberOfSamples = std::numeric_limits<size_t>::max());
                };
            }
        }
    }
}
