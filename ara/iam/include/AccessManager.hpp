#pragma once
#include "types.hpp"
#include "IPCserverInterface.hpp"
#include "Grant.hpp"

namespace ara
{
    namespace iam
    {
        class AccessManager
        {
            private:
            static std::map<std::string, std::vector<ara::iam::Grant>> AccessMap;
            void ParseJson(std::string filePath);
            
            public: 
            bool InitGrantStorage(ara::iam::GrantStorage & grantStorage, std::string basePath);
            bool InitServerAdapter(ara::iam::IPCserverInterface & server);

            void RunEventLoop();
        };
    }
}

