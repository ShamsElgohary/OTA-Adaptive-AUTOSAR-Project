#ifndef _ARA_COM_INTERNAL_FIELDPROXY_HPP_
#define  _ARA_COM_INTERNAL_FIELDPROXY_HPP_

#include "Proxy.hpp"

namespace ara
{
    namespace com
    {
        namespace internal
        {
            template <typename T>
            class FieldProxy {

                private: 
                    Proxy::HandleType Handle;
                    uint16_t F_ID;

                public:

                    FieldProxy(Proxy::HandleType& Handle, uint16_t F_ID)
                    {
                        this->Handle = Handle;
                        this->F_ID = F_ID;
                    }

                public:

                    void Subscribe(size_t maxSampleCount);

                    ara::core::Result<size_t> GetFreeSampleCount() const noexcept;

                    ara::com::SubscriptionState GetSubscriptionState() const;

                    void Unsubscribe();

                    void SetReceiveHandler(ara::com::EventReceiveHandler handler);

                    void UnsetReceiveHandler();

                    void SetSubscriptionStateChangeHandler(ara::com::SubscriptionStateChangeHandler handler);

                    void UnsetSubscriptionStateChangeHandler();

                    template <typename F>
                    ara::core::Result<size_t> GetNewSamples(F&& f,
                    size_t maxNumberOfSamples = std::numeric_limits<size_t>::max());


                    ara::core::Future<T> Get();

                    ara::core::Future<T> Set(const T& value);
            };
        }
    }
}

#endif