#pragma once

#include <OVR_CAPI.h>

#include "RiftDotNet.h"
#include "DeviceInfo.h"
#include "FovPort.h"


namespace RiftDotNet
{
	namespace Platform
	{
		public ref class HMDInfo sealed
			: public DeviceInfo
			, public IHMDInfo
		{
		public:
			property unsigned int HResolution { virtual unsigned int get() { return _hResolution; } }
			property unsigned int VResolution  { virtual unsigned int get() { return _vResolution; } }
			property int       DesktopX { virtual int get() { return _desktopX; } }
			property int DesktopY { virtual int get() { return _desktopY; } }
			property Object^ DisplayDevice { virtual Object^ get() { return _displayDevice; } }
			property unsigned int  Caps { virtual unsigned int get() { return _caps; } }
			property unsigned int  DistortionCaps { virtual unsigned int get() { return _dCaps; } }
			property array<IFovPort^>^ DefaultEyeFov { virtual array<IFovPort^>^ get() { return _defaultEyeFov; }}
			property array<IFovPort^>^ MaxEyeFov { virtual array<IFovPort^>^ get() { return _maxFov; }}
			property array<EyeType>^ EyeRenderOrder { virtual array<EyeType>^ get() { return _eyeRenderOrder; }}


			HMDInfo()
				: DeviceInfo(DeviceType::HMD)
				, _hResolution(0)
				, _vResolution(0)
				, _desktopX(0)
				, _desktopY(0)
				, _displayDevice(nullptr)
				, _caps(0)
				, _dCaps(0)
			{
				_defaultEyeFov = gcnew array<IFovPort^>(2);
				_maxFov = gcnew array<IFovPort^>(2);
				_eyeRenderOrder = gcnew array<EyeType>(2);
			}

			HMDInfo(const ovrHmdDesc& native)
				: DeviceInfo(native)
				, _hResolution(native.Resolution.w)
				, _vResolution(native.Resolution.h)
				, _desktopX(native.WindowsPos.x)
				, _desktopY(native.WindowsPos.y)
				, _displayDevice(gcnew String(native.DisplayDeviceName))//< TODO: MacOS...
				, _caps(native.Caps)
				, _dCaps(native.DistortionCaps)
			{
				_defaultEyeFov = gcnew array<IFovPort^>(2);
				_maxFov = gcnew array<IFovPort^>(2);
				_eyeRenderOrder = gcnew array<EyeType>(2);

				for (size_t i = 0; i < 2; i++)
				{
					_defaultEyeFov[i] = gcnew FovPort(native.DefaultEyeFov[i]);
					_maxFov[i] = gcnew FovPort(native.MaxEyeFov[i]);
					_eyeRenderOrder[i] = (EyeType)native.EyeRenderOrder[i];
				}
			}

		private:
			const unsigned int _hResolution, _vResolution;
			const int       _desktopX, _desktopY;
			Object^ _displayDevice;
			unsigned int _caps;
			unsigned int _dCaps;
			array<IFovPort^>^ _defaultEyeFov;
			array<IFovPort^>^ _maxFov;
			array<EyeType>^ _eyeRenderOrder;
		};
	}
}
