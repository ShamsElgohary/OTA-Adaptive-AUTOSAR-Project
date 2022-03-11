#include <iostream>
#include <sstream>
#include <string>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace iam
{
    typedef uint16_t ServiceID;
    typedef uint16_t InstanceID;
    typedef std::string process_name;
}

