#pragma once

#include "UCM_Common.hpp"
#include "State.hpp"

#include "com/comTypes.hpp"
#include "core/Future.hpp"

namespace ara
{
    namespace com
    {
        namespace proxy
        {
                class PackageManagerProxy
                {
                public:

                /* IDENTIFIES THE SERVICE & CONTAINS NEEDED ADDRESSING INFORMATION (IN PROXY) */
                class HandleType
                {
                public:
                    bool operator==(const HandleType& other) const;
                    HandleType& operator=(const HandleType& other);
                    const ara::com::InstanceIdentifier& GetInstanceId() const;
                    
                private:

                };

                
                static ara::com::FindServiceHandle StartFindService(
                ara::com::FindServiceHandler<PackageManagerProxy::HandleType> handler,
                ara::com::InstanceIdentifier instanceId);


                /* METHOD TO STOP FINDING SERVICE REQUEST*/
                static void StopFindService(ara::com::FindServiceHandle handle);

                /*
                * Opposed to StartFindService(handler, instance) this version
                * is a "one-shot" find request, which is:
                * - synchronous, i.e. it returns after the find has been done
                and a result list of matching service instances is
                *
                available. (list may be empty, if no matching service
                *
                instances currently exist) */
                static ara::com::ServiceHandleContainer<PackageManagerProxy::HandleType> FindService(ara::com::InstanceIdentifier instanceId);


                /* EXPLICIT CONSTRUCTOR */
                explicit PackageManagerProxy(HandleType &handle);

                
                PackageManagerProxy(PackageManagerProxy &other) = delete;
                PackageManagerProxy& operator=(const PackageManagerProxy &other) = delete;


                private:

		    	};
            }
        }
    }
