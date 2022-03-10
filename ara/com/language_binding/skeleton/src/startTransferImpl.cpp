#include "../include/startTransfer_skeleton.h"
//include someip files
using namespace ara::com::skeleton;

class UCM_startTransferImpl : public UCM_Skeleton
{
public:
    void setID(uint32_t id) { this->instanceId = id; }
    void setMode(ara::com::MethodCallProcessingMode mode) { this->mode = mode; }
    
    UCM_Skeleton(ara::com::InstanceIdentifier instanceId,
                 ara::com::MethodCallProcessingMode mode)
    {
        this->setID(instanceId);
        this->setMode(mode);
    }
    void OfferService()
    {
        //uniqueness check
        if (this->ptr2bindingProtocol->protocol == ara::com::BindingProtocol::SOMEIP)
        {
            OfferServiceSomeIP(this->instanceId,port,ip,this->ptr2bindingProtocol);
            while(1)
            {
                receive(); //gowaha listen to port
            }
            
        }
        else
        {
            OfferServiceDDS(this->instanceId);
        }
    }

    void StopOfferService()
    {
        if (this->ptr2bindingProtocol->protocol == ara::com::BindingProtocol::SOMEIP)
        {
            StopOfferServiceSomeIP(this->instanceId);
        }
        else
        {
            StopOfferServiceDDS(this->instanceId);
        }
    }
};