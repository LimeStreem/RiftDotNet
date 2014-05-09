using System;
using RiftDotNet.Win32;

namespace RiftDotNet
{
	internal sealed class DeviceResources
		: IDisposable
	{
		public readonly IHMDDevice Device;
		public readonly IHMDInfo Info;
		public readonly ISensorState Sensor;
		public readonly DeviceKey Key;

        public DeviceResources(IFactory factory, IHMDDevice handle)
		{
			if (factory == null || handle == null)
				throw new ArgumentNullException();

			Info = handle.Info;
            Device = handle;
			Sensor = Device.GetSensorState(0.0);

			if (Info == null)
				throw new ArgumentNullException();
			if (Device == null)
				throw new ArgumentNullException();
            if (Sensor == null)
                System.Diagnostics.Trace.TraceWarning("Unable to create Sensor");

            if (Sensor != null)
                Key = new DeviceKey(Device.SensorInfo);
		}

		#region IDisposable Members

		public void Dispose()
		{
			Device.Dispose();
		}

		#endregion
	}
}