#pragma once

#include "types.hpp"
#include "GrantStorage.hpp"
//#include "IPCserverInterface.hpp"


namespace ara
{
    namespace iam
    {
        class AccessManager
        {
            public: 
            
            static void InitGrantStorage(std::string basePath);
            
            //static void InitServerAdapter(ara::iam::IPCserverInterface & server);

            static void RunEventLoop();
        };
    }
}

