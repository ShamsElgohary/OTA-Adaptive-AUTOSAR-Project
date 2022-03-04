#include <future>
#include "ara/sm/SM.hpp"
#include "ara/com/types.hpp"
namespace ara
{
    namespace com
    {
        namespace skeleton
        {
            class UpdateRequestSkeleton
            {
            public:
                /**
    * Ctor taking instance identifier as parameter and having default
    request processing mode kEvent.
    */
                UpdateRequestSkeleton(ara::com::InstanceIdentifier instance,
                                      ara::com::MethodCallProcessingMode mode = ara::com::
                                          MethodCallProcessingMode::kEvent);

                /* skeleton instances are nor copy constructible
                 */
                UpdateRequestSkeleton(const UpdateRequestSkeleton &other) = delete;

                /* skeleton instances are nor copy assignable
                 */
                UpdateRequestSkeleton &operator=(const UpdateRequestSkeleton &other) = delete;
                /**
                 * The Communication Management implementer should care in his dtor
                 * implementation, that the functionality of StopOfferService()
                 * is internally triggered in case this service instance has
                 * been offered before. This is a convenient cleanup functionality.
                 **/
                ~UpdateRequestSkeleton();
                /**
                 * Offer the service instance.
                 * method is idempotent - could be called repeatedly.
                 **/
                void OfferService();

                /**
    * Stop Offering the service instance.
    * method is idempotent - could be called repeatedly.
    *
    * if service instance gets destroyed - it is expected that the
    Communication Management implementation
    * calls StopOfferService() internally.
    **/
                void StopOfferService();
                /**
                 * For all output and non-void return parameters
                 * an enclosing struct is generated, which contains
                 * non-void return value and/or out parameters.
                 */
                struct StartUpdateSessionOutput
                {
                    uint8_t result;
                };
                struct VerifyUpdateOutput
                {
                    uint8_t result;
                };
                struct PrepareUpdateOutput
                {
                    uint8_t result;
                };

                /**
    * This fetches the next call from the Communication Management and
    executes it.
    * Only available in polling mode. In event mode it shall throw an
    exception.
    */
                std::future<bool> ProcessNextMethodCall();

                // All methods are pure virtual and have to be implemented

                virtual std::future<StartUpdateSessionOutput> StartUpdateSession() = 0;
                virtual void StopUpdateSession() = 0;
                virtual std::future<VerifyUpdateOutput> VerifyUpdate(ara::sm::FunctionGroupList Functiongroups) = 0;
                virtual std::future<PrepareUpdateOutput> PrepareUpdate(ara::sm::FunctionGroupList Functiongroups) = 0;
            };
        }
    }
}