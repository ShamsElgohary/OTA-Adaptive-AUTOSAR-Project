#include "../include/IAMGrantQuery.hpp"

bool ara::iam::IAMGrantQuery::HasGrant(Grant& G)
{
    std::uint8_t rtn = ara::iam::IAMGrantQuery::IPC.Connect();
    if (rtn == 0)
    {
        clientPID = getpid();
        std::cout << "[iamClient] " << clientPID << std::endl;
        IPC.sendPID(clientPID);
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


