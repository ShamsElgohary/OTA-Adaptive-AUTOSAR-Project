#include "../include/IAMGrantQuery.hpp"

bool ara::iam::IAMGrantQuery::HasGrant(Grant& G)
{
    std::uint8_t rtn = ara::iam::IAMGrantQuery::IPC.Connect();
    if (rtn)
    {
        IPC.Send(G);
        return IPC.Receive();
    }
    else
    {
        return false;
    }
}

ara::iam::IAMGrantQuery::IAMGrantQuery()
{
    
}


