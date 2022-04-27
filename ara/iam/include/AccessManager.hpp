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
            ara::iam::IPCserverInterface server;
            ara::exec::FindProcessClient FPC;

            public: 
            
            void InitGrantStorage(std::string basePath);
            
            std::uint8_t InitServerAdapter();

            void RunEventLoop();
        };
    }
}

