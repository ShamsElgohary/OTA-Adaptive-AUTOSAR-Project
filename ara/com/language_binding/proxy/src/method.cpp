#include "../include/method.hpp"

using namespace std;

namespace ara
{
    namespace com
    {
        namespace proxy
        {
            namespace method
            {
                template <typename output, typename... Params>
                output MethodBase<output, Params...>::operator()(Params... args)
                {
                    ara::com::proxy::method::MethodBase::Delegate->SendRequest(this->ID, args...);
                    output in ;
                    ara::com::proxy::method::MethodBase::Delegate->receive(in);
                    return in;
                }
                template <typename output, typename... Params>
                MethodBase<output, Params...>::MethodBase(std::shared_ptr<ara::com::NetworkBindingBase>ptr, uint16_t id) : Delegate(ptr), ID(id)
                {}
            }
        }
    }
}
