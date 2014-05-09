#pragma once

#include "RiftDotNet.h"
#include "DeviceManager.h"
#include "HMDInfo.h"
#include "UnitTestDetector.h"
#include "SensorState.h"

namespace RiftDotNet
{
	namespace Platform
	{
		/// <summary>
		/// Internal IFactory implementation.
		/// There can only be one factory and it takes care of initializing the
		/// OVR system and creates the necessary objects.
		/// </summary>
		public ref class Factory sealed
			: public ITestingFactory
		{
		public:

			virtual IHMDInfo^ CreateHMDInfo()
			{
				return gcnew HMDInfo();
			}

			virtual ISensorState^ CreateSensorState()
			{
				return gcnew SensorSate(ovrSensorState());
			}

			virtual IDeviceManager^ CreateDeviceManager()
			{
				return gcnew DeviceManager();
			}

			static property Factory^ Instance { Factory^ get() { return _theOne; } }

			static Factory()
			{
				// For a reason I cannot fathom, the fucking
				// resharper/nunit test runner refuses to load log4net.
				// Well, if it aint gonna behave, then nobody is getting logging...
				if (UnitTestDetector::IsRunningFromNunit)
				{
					ovr_Initialize();
				}
				else
				{
					// Yeah, who's gonna delete that?
					auto log = new Log4Net();
					ovr_Initialize();
				}

				_theOne = gcnew Factory();
			}

		private:

			Factory()
			{}

		private:

			static Factory^ _theOne;
		};
	}
}