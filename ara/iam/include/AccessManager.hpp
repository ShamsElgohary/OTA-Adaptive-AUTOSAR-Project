#pragma once

#include "types.hpp"
//#include "IPCserverInterface.hpp"
#include "Grant.hpp"


namespace ara
{
    namespace iam
    {
        class AccessManager
        {
            private:
            //static std::map<std::string, std::vector<ara::iam::Grant>> AccessMap;
            static void ParseJson(std::string filePath);
            
            public: 
            static bool InitGrantStorage(std::string basePath);
            //static bool InitServerAdapter(ara::iam::IPCserverInterface & server);

            static void RunEventLoop();
        };
    }
}

