#ifndef _ARA_COM_INTERNAL_METHODSKELETON_HPP_
#define  _ARA_COM_INTERNAL_METHODSKELETON_HPP_

#include "COM_Types.hpp"
#include "CORE_Types.hpp"

namespace ara
{
    namespace com
    {
        namespace internal
        {
            template <typename Output>
            class MethodSkeleton {

                public:
                
                /* VARIADIC FUNCTIONS EXPLAINED WELL @ https://www.geeksforgeeks.org/variadic-function-templates-c/ */
                template <typename T, typename... Types>
                ara::core::Future<Output> operator()(T var1, Types... var2);
            };
        }
    }
}

#endif