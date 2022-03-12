#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>


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

