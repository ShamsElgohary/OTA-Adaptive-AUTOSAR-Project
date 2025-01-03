#pragma once
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cstdint>
#include <utility>
#include <bits/stdc++.h>
#include <boost/filesystem.hpp>


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "../../../../utility/general.hpp"
#include "../../../../utility/simulation/include/simulation.hpp"
#include "../../../../utility/jsoncpp/header/json.h"


namespace ara
{
    namespace iam
    {
        typedef uint16_t ServiceID;
        typedef uint16_t InstanceID;
        typedef uint32_t PID;
        typedef std::string Process_Name;
        typedef std::string Grant_Type;
        typedef std::string PR_Type;
        
        std::string convertToString(char* a, int size);
    }
}

