#include "future"
#include "ara/sm/SM.hpp"
namespace ara
{
    namespace com
    {
        namespace proxy
        {
            namespace methods
            {
                // for proxy
                class StartUpdateSession
                {
                    struct output
                    {
                        uint8_t success;
                    };
                    std::future<output> operator()(void);
                };
                class StopUpdateSession
                {
                    struct output
                    {
                    };
                    std::future<void> operator()(void);
                };
                class PrepareUpdate
                {
                    struct output
                    {
                        uint8_t success;
                    };
                    std::future<output> operator()(ara::sm::FunctionGroupList functiongroups);
                };
                class VerifyUpdate
                {
                    struct output
                    {
                        uint8_t success;
                    };
                    std::future<output> operator()(ara::sm::FunctionGroupList functiongroups);
                };

            }
        }
    }
}
