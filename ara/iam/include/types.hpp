#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cstdint>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>



namespace ara
{
    namespace iam
    {
        typedef uint16_t ServiceID;
        typedef uint16_t InstanceID;
        typedef std::string Process_Name;
        typedef std::string Grant_Type;
        typedef std::string PR_Type;
    }
}

