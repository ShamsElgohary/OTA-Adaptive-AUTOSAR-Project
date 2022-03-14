#pragma once
#include <iostream>
#include "../../../language_binding/Services/include/serviceProxy.hpp"
using namespace std;

namespace ara
{
    namespace com
    {
        namespace skeleton
        {
            namespace method
            {
                struct input
                {
                };

                struct output
                {
                };

                template <typename Tin, typename Tout>
                class methodBase
                {
                public:
                    virtual Tout processMethod(Tin &ip);
                };
            }
        }
    }
}
