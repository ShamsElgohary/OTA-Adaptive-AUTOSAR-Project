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
                output MethodBase::operator()(Params... args)
                {
                    this->Delegate->SendRequest(this->ID, args...);
                    output in ;
                    this->Delegate->ReceiveMessage(in);
                    return in;
                }
                
                MethodBase::MethodBase(std::shared_ptr<ara::com::NetworkBindingBase>ptr, uint16_t id) : Delegate(ptr), ID(id)
                {}
            }
        }
    }
}
