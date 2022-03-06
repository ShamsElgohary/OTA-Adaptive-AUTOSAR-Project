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
            namespace FieldBase
            {
                template <typename T>
                class FieldBase {

                    private: 
                        ara::com::proxy::ProxyBase::HandleType Handle;
                        uint16_t F_ID;

                    public:
                        FieldProxy(ara::com::proxy::ProxyBase::HandleType& Handle, uint16_t F_ID)
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
}
