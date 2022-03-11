#include "IAM_types.h"
#include "IPCclientInterface.hpp"
using namespace iam;

class IAMGrantQuery
{
public:
    IPCclientInterface IPC;
    bool HasGrant(Grant G);
};