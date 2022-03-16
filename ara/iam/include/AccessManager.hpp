#pragma once

#include "../lib/include/types.hpp"
#include "../lib/include/Grant.hpp"
#include "GrantStorage.hpp"
#include "IPCserverInterface.hpp"
#include "../../exec/include/find_process_client.hpp"


namespace ara
{
    namespace iam
    {
        class AccessManager
        {
            private:
            static ara::iam::IPCserverInterface server;

            public: 
            
            static void InitGrantStorage(std::string basePath);
            
            static std::uint8_t InitServerAdapter();

            static void RunEventLoop();
        };
    }
}

