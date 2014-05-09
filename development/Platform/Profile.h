#pragma once

#include <OVR_Device.h>
#include <OVR_Profile.h>

#include "RiftDotNet.h"
#include "DeviceInfo.h"


namespace RiftDotNet
{
	namespace Platform
	{
		public ref class Profile sealed
			: public IProfile
		{
		private:
			float _PlayerHeight;
			float _IPD;

		public:

			property float PlayerHeight { virtual float get() { return _PlayerHeight; } }
			property float IPD { virtual float get() { return _IPD; } }			
			
			Profile(OVR::Profile *native)
			{
				_PlayerHeight = native->GetPlayerHeight();
				_IPD = native->GetIPD();
			}

		};
	}
}
