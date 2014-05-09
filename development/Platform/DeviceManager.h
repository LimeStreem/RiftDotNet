#pragma once

#include <OVR_CAPI.h>

#include "RiftDotNet.h"
#include "System.h"
#include "HMDDevice.h"

using namespace System::Collections::Generic;

namespace RiftDotNet
{
	namespace Platform
	{
		public ref class DeviceManager
			:  public IDeviceManager
		{
		private:
			IntPtr _equalityHandle;

		public:
			DeviceManager() 
			{
			}

			~DeviceManager()
			{
				// free all allocated memory from OVR SDK
				ovr_Shutdown();
			}

			property  DisposableArray<IHMDDevice^>^ HMDDevices
			{
				virtual  DisposableArray<IHMDDevice^>^ get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("IDeviceHandle");

					int numDevices = ovrHmd_Detect();
					auto ret = gcnew List<IHMDDevice^>(numDevices);

					// get all devices
					for (int i = 0; i < numDevices; i++)
					{
						ovrHmd hmd = ovrHmd_Create(i);
						ret->Add(gcnew HMDDevice(hmd));
					}

					return gcnew DisposableArray<IHMDDevice^>(ret->ToArray());
				}
			}

			property bool IsDisposed
			{
				virtual bool get()
				{
					return false;
				}
			}

			virtual bool Equals(Object^ other) override sealed
			{
				if (other == nullptr)
					return false;

				auto tmp = dynamic_cast<DeviceManager^>(other);
				if (tmp == nullptr)
					return false;

				// For now, I will assume that the ver same OVR::DeviceBase pointer
				// is used for the same device. But maybe we need to compare the device
				// id or something similar...
				return _equalityHandle == tmp->_equalityHandle;
			}

			virtual int GetHashCode() override sealed
			{
				return _equalityHandle.GetHashCode();
			}

			virtual bool Equals(IDevice^ other) sealed
			{
				return Equals((Object^)other);
			}

			property DeviceType Type
			{
				virtual DeviceType get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("IDevice");

					return DeviceType::Manager;
				}
			}

		private:
		};
	}
}