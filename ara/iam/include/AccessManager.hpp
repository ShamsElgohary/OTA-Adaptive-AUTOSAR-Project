#pragma once

#include "types.hpp"
#include "GrantStorage.hpp"
#include "IPCserverInterface.hpp"


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

