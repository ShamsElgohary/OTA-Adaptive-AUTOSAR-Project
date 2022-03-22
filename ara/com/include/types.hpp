#pragma once 
#include<iostream>
#include<string>
#include <stdint.h>
#include <memory>
#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

namespace ara
{
    namespace com
    {
        enum class MethodCallProcessingMode { kPoll, kEvent, kEventSingleThread };
        using serviceIdentifierType = uint32_t; 
        using InstanceIdentifier = uint32_t;
        class InstanceIdentifierContainer : public std::vector<InstanceIdentifier>{};
        template <typename H>
        class ServiceHandleContainer : public std::vector<H>{};
    }
}