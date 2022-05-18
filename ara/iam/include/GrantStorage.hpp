#pragma once
#include "../lib/include/types.hpp"
#include "../lib/include/Grant.hpp"



namespace ara
{
    namespace iam
    {
        class GrantStorage
        {
            private:
                static std::map<std::string, std::vector<ara::iam::Grant>> AccessMap;

            public:
                static void ParseJson(std::string filePath);

                static bool SearchGrantStorage (const std::string P_name, const ara::iam::Grant & G);
        };
    }
}