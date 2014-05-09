#pragma once

using namespace System;

#include "RiftDotNet.h"
#include <OVR_CAPI.h>

namespace RiftDotNet
{
	namespace Platform
	{
		public ref class DeviceInfo
			: public IDeviceInfo
		{
		public:
			property DeviceType InfoClassType { virtual DeviceType get() { return _infoClassType; } }
			property DeviceType Type { virtual DeviceType get() { return _type; } }
			property String^ ProductName { virtual String^ get() { return _productName; } }
			property String^ Manufacturer { virtual String^ get() { return _manufacturer; } }

		private:

			const DeviceType _infoClassType;
			const DeviceType _type;
			String^ _productName;
			String^ _manufacturer;

		public:

			DeviceInfo(DeviceType type)
				: _infoClassType(DeviceType::None)
				, _type(type)
				, _productName(nullptr)
				, _manufacturer(nullptr)
			{
				if (type == DeviceType::None)
					throw gcnew ArgumentException("ovrHmd_None is not allowed");
			}

			DeviceInfo(const ovrHmdDesc& native)
				: _infoClassType((DeviceType)native.Type)
				, _type((DeviceType)native.Type)
				, _productName(gcnew String(native.ProductName))
				, _manufacturer(gcnew String(native.Manufacturer))
			{}
		};
	}
}