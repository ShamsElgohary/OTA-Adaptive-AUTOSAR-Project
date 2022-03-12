#pragma once
#include "types.hpp"
#include "IPCclientInterface.hpp"
#include "Grant.hpp"

namespace ara
{
    namespace iam
    {
        class IAMGrantQuery
        {
            private:
            IPCclientInterface *IPC;

            public:
            bool HasGrant(Grant& G);

            IAMGrantQuery();
        };
    }
}

