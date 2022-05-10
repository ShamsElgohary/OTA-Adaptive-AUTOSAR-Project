#pragma once
#include "../lib/include/types.hpp"
#include "../lib/include/Grant.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <boost/filesystem.hpp>
#include "../../../utility/jsoncpp/header/json.h"
#include "../../../utility/simulation/include/simulation.hpp"


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