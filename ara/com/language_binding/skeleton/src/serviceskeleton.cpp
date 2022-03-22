#include "../include/serviceskeleton.hpp"

using namespace std;

bool stopOfferFlag = false;

namespace ara
{
    namespace com
    {
        namespace skeleton
        {
            skeletonBase::skeletonBase(ara::com::serviceIdentifierType serviceID ,ara::com::InstanceIdentifier instanceId, ara::com::MethodCallProcessingMode mode)
            :instanceID{instanceID} , serviceID{serviceID} ,mode{mode}{
                //parsing to get ip and port type of network binding
                this->ptr2bindingProtocol = std::make_shared<SomeIpNetworkBinding>(serviceID,instanceID,ip,port);
            }
            void skeletonBase::OfferService()
            {
                this->ptr2bindingProtocol->OfferService();
                serve();
            }
            void skeletonBase::serve()
            {
                while (1)
                {
                    if (stopOfferFlag == true)
                    {
                        break;
                    }
                    handleMethod( this->ptr2bindingProtocol->get_method_id() );
                }
            }
            void skeletonBase::StopOfferService()
            {
                this->ptr2bindingProtocol->StopOfferService();
            }
        }
    }
}


// to be generated
// void Serviceskeleton::handleMethod(int id)
// {
//     switch (id)
//     {
//     case 1:
//         input_add in;
//         this->ptr2bindingProtocol->receive(in.i , in.y);
//         int out = add(in.i, in.y);
//         this->ptr2bindingProtocol->send(out);
//         break;
//     }
// }


