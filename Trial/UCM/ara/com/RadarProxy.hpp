#ifndef _COM_MYCOMPANY_RADARPROXY_HPP_
#define _COM_MYCOMPANY_RADARPROXY_HPP_

#include "Proxy.hpp"
#include "EventProxy.hpp"
#include "MethodProxy.hpp"
#include "FieldProxy.hpp"

namespace com
{
	namespace myCompany
	{
		namespace Proxy
		{	
			namespace events
			{
				class BrakeEvent : public ara::com::internal::EventProxy
				{
					public:
						BrakeEvent(ara::com::internal::Proxy::HandleType& Handle) : EventProxy(Handle, 1)
						{}
				};
			} 
			
			
			namespace methods
			{
				struct CalibrateOutput
				{
					/* data */
				};
				
				class Calibrate : public ara::com::internal::MethodProxy <CalibrateOutput>
				{
					public:
						Calibrate(ara::com::internal::Proxy::HandleType& Handle) : MethodProxy(Handle, 2)
						{}
				};

				struct AdjustOutput
				{
					/* data */
				};

				class Adjust : public ara::com::internal::MethodProxy <AdjustOutput>
				{
					public:
						Adjust(ara::com::internal::Proxy::HandleType& Handle) : MethodProxy(Handle, 3)
						{}
				};

			}


			namespace fields
			{

			}

			class RadarProxy : public ara::com::internal::Proxy
			{
				private:
				

				public:
					RadarProxy(HandleType &handle) : Proxy (handle)
					{
						events::BrakeEvent BrakeEvent(handle);
						methods::Calibrate Calibrate(handle);
						methods::Adjust Adjust(handle);
					}
			};
		} 
	}
}
#endif
