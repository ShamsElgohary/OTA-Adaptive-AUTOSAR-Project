#include "IAM_types.h"
#include "IPCclientInterface.hpp"
using namespace iam;

class IAMGrantQuery
{
    IPCclientInterface *IPC;
public:
    bool HasGrant(Grant& G)
    {
        auto stream  = G.serialize_write()
        Ipc.send(stream);
    }
    IAMGrantQuery()
    {
        Ipc = new IPCclientInterface() ;
    }
};

