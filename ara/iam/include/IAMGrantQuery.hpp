#pragma once
#include "types.h"
#include "IPCclientInterface.hpp"

namespace ara
{
    namespace iam
    {
        class IAMGrantQuery
        {
            IPCclientInterface *IPC;

            public:
            bool HasGrant(Grant& G);

            IAMGrantQuery();
        };
    }
}

