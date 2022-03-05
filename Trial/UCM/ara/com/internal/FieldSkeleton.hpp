#ifndef _ARA_COM_INTERNAL_FIELDSKELETON_HPP_
#define  _ARA_COM_INTERNAL_FIELDSKELETON_HPP_

#include "COM_Types.hpp"
#include "CORE_Types.hpp"

namespace ara
{
    namespace com
    {
        namespace internal
        {
            template <typename T>
            class FieldSkeleton {

                public:
                    void Update(const T& data);

                    void RegisterGetHandler(std::function<ara::core::Future<T>()>getHandler);

                    void RegisterSetHandler(std::function<ara::core::Future<FieldType>(const FieldType& data)> setHandler);

            };
        }
    }
}

#endif