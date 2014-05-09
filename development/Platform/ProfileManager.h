#pragma once

#include <OVR_Device.h>
#include <OVR_DeviceImpl.h>
#include <OVR_Profile.h>

#include "RiftDotNet.h"
#include "System.h"
#include "DeviceBase.h"
#include "DeviceHandle.h"
#include "DeviceEnumerable.h"
#include "HMDDevice.h"
#include "SensorDevice.h"
#include "ValueWrapper.h"
#include "Profile.h"

using namespace System::Collections::Generic;



namespace RiftDotNet
{
	namespace Platform
	{
		public ref class ProfileManager
			: public IProfileManager
		{
		private:
			OVR::ProfileManager* native;

		public:
			
			ProfileManager(OVR::ProfileManager* native)
			{
				this->native = native;
			}

			~ProfileManager()
			{
				
			}

			property RiftDotNet::IProfile^ GetDeviceDefaultProfile
			{
				virtual RiftDotNet::IProfile^ get() override
				{
					//if (IsDisposed)
					//	throw gcnew ObjectDisposedException("IProfile");
					/*
					OVR::DeviceInfo info;
					GetNative<OVR::DeviceManager>()->GetDeviceInfo(&info);
					ret->Add(gcnew TypedDeviceHandle<IHMDDevice^,IHMDInfo^>(wrapper));
					return gcnew RiftDotNet::Platform::DeviceInfo(info);
					*/
					return (RiftDotNet::IProfile^)NULL;
				}
			}

		private:
		};
	}
}