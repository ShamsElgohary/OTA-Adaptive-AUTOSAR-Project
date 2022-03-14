#pragma once
#include <iostream>
#include"../include/serviceProxy.hpp"
using namespace std;

namespace ara
{
    namespace com
    {
        namespace proxy
        {
            class ProxyBase
            {

                ProxyBase(HandleType handle)
                {
                    ProxyBase::Delegate = HandleTypeFactory::create(handle.GetBindingProtocol());
                    Delegate.function;
                }

                static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService()
                {
                    return SD::FindService();
                }

                static ara::com::ServiceHandleContainer<ProxyBase::HandleType> FindService(ara::com::InstanceIdentifier instanceId)
                {
                    return SD::FindService(instanceId);
                }
                // class handle
                // {

                // };
            };
        }
    }
}
