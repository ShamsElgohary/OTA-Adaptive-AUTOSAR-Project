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
            ara::iam::IPCclientInterface IPC;
            int clientPID;
            public:
            bool HasGrant(Grant& G);
            IAMGrantQuery();
        };
    }
}

