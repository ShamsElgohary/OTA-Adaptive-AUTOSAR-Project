#pragma once 
#include <iostream>
#include "serviceProxy.hpp"
#include "event.hpp"

using namespace std ;

namespace ara 
{
    namespace com
    {
        namespace proxy 
        {
            namespace field
            {                
                template <typename T>
                class FieldBase : public proxy::event::EventBase<T>
                {
                    public:
                        /* MUST */
                        ara::core::Future<T> Get();

                        /* MUST */
                        ara::core::Future<T> Set(const T& value);
                };
                
                /* template <typename T, bool hasGetter, bool hasNotifier, bool hasSetter>
                class FieldBase : FieldBase<T, hasNotifier, hasGetter, hasSetter>
                {};

                template <typename T>
                class FieldBase<T,true,true,true>  : public Field<T>
                {};

                template <typename T>
                class FieldBase<T,true,true,false> : public Field<T>
                {
                    public:
                        ara::core::Future<T> Set(const T& value) = delete;
                };

                template <typename T>
                class FieldBase<T,false,true,true> : public Field<T>
                {
                    public:
                        ara::core::Future<T> Get() = delete;
                };

                template <typename T>
                class FieldBase<T,false,true,false> : public Field<T>
                {
                    public:
                        ara::core::Future<T> Set(const T& value) = delete;
                        ara::core::Future<T> Get() = delete;
                    
                };

                template <typename T>
                class FieldBase<T,true,false,true> : public Field<T>
                {
                    public:
                        void Subscribe(size_t maxSampleCount);

                        ara::com::SubscriptionState GetSubscriptionState() const = delete;

                        void Unsubscribe() = delete;

                        ara::core::Result<size_t> GetFreeSampleCount() const noexcept = delete;

                        void SetReceiveHandler(ara::com::EventReceiveHandler handler) = delete;

                        void UnsetReceiveHandler() = delete;

                        void SetSubscriptionStateChangeHandler(ara::com::SubscriptionStateChangeHandler handler) = delete;

                        void UnsetSubscriptionStateChangeHandler() = delete;

                        template <typename F>
                        ara::core::Result<size_t> GetNewSamples(F&& f,
                        size_t maxNumberOfSamples = std::numeric_limits<size_t>::max()) = delete;
                    
                };

                template <typename T>
                class FieldBase<T,true,false,false> : public Field<T>
                {
                    public:
                        void Subscribe(size_t maxSampleCount);

                        ara::com::SubscriptionState GetSubscriptionState() const = delete;

                        void Unsubscribe() = delete;

                        ara::core::Result<size_t> GetFreeSampleCount() const noexcept = delete;

                        void SetReceiveHandler(ara::com::EventReceiveHandler handler) = delete;

                        void UnsetReceiveHandler() = delete;

                        void SetSubscriptionStateChangeHandler(ara::com::SubscriptionStateChangeHandler handler) = delete;

                        void UnsetSubscriptionStateChangeHandler() = delete;

                        template <typename F>
                        ara::core::Result<size_t> GetNewSamples(F&& f,
                        size_t maxNumberOfSamples = std::numeric_limits<size_t>::max()) = delete;

                        ara::core::Future<T> Set(const T& value) = delete;
                };

                template <typename T>
                class FieldBase<T,false,false,true> : public Field<T>
                {
                    public:
                        void Subscribe(size_t maxSampleCount);

                        ara::com::SubscriptionState GetSubscriptionState() const = delete;

                        void Unsubscribe() = delete;

                        ara::core::Result<size_t> GetFreeSampleCount() const noexcept = delete;

                        void SetReceiveHandler(ara::com::EventReceiveHandler handler) = delete;

                        void UnsetReceiveHandler() = delete;

                        void SetSubscriptionStateChangeHandler(ara::com::SubscriptionStateChangeHandler handler) = delete;

                        void UnsetSubscriptionStateChangeHandler() = delete;

                        template <typename F>
                        ara::core::Result<size_t> GetNewSamples(F&& f,
                        size_t maxNumberOfSamples = std::numeric_limits<size_t>::max()) = delete;
                        ara::core::Future<T> Get() = delete;
                }; */
            }
        }
    }
}
