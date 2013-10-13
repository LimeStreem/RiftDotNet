﻿namespace RiftDotNet
{
	/// <summary>
	///     DeviceManager maintains and provides access to devices supported by OVR, such as
	///     HMDs and sensors. A single instance of DeviceManager is normally created at
	///     program startup, allowing devices to be enumerated and created.
	/// </summary>
    public interface IProfileManager
	{
		/// <summary>
		/// The list of all HMD devices, one for each HMD, currently
		/// attached to this system.
		/// </summary>
        IProfile GetDeviceDefaultProfile { get; }
	}
}