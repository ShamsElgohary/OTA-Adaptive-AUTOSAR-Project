#include "../include/IAMGrantQuery.hpp"

bool ara::iam::IAMGrantQuery::HasGrant(Grant& G)
{
    std::uint8_t rtn = ara::iam::IAMGrantQuery::IPC.Connect();
    if (rtn == 0)
    {
        clientPID = getpid();
        IPC.sendPID(clientPID);
        IPC.Send(G);

        return IPC.Receive();
    }
    else
    {
        std::cout << "7aga" << std::endl;
        return false;
    }
}

ara::iam::IAMGrantQuery::IAMGrantQuery()
{
    
}


