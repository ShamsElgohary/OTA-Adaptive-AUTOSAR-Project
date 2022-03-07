#ifndef _ARA_CORE_CORE_TYPES_HPP_
#define _ARA_CORE_CORE_TYPES_HPP_

#include <future>

namespace ara
{
    namespace core
    {
        template <typename T>
        class Future : public std::future <T>
        {
            public:
            
            
        };

        template <typename T>
        class Promise : public std::promise <T>
        {
            public:
            
            
        };

        template <typename T>
        class Result 
        {

        };


    }
}

#endif
