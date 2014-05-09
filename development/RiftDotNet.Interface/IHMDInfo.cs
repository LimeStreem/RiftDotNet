namespace RiftDotNet
{
	/// <summary>
	/// This structure describes various aspects of the HMD allowing us to configure rendering.
	///
	///  Currently included data:
	///   - Physical screen dimensions, resolution, and eye distances.
	///     (some of these will be configurable with a tool in the future).
	///     These arguments allow us to properly setup projection across HMDs.
	///   - DisplayDeviceName for identifying HMD screen; system-specific interpretation.
	///
	/// </summary>
	public interface IHMDInfo
		: IDeviceInfo
	{
		/// <summary>
		/// Size of the entire screen, in pixels.
		/// </summary>
		uint HResolution { get; }

		/// <summary>
		/// Size of the entire screen, in pixels.
		/// </summary>
		uint VResolution { get; }

		/// <summary>
		/// Desktop coordinate position of the screen (can be negative; may not be present on all platforms)
		/// </summary>
		int DesktopX { get; }

		/// <summary>
		/// Desktop coordinate position of the screen (can be negative; may not be present on all platforms)
		/// </summary>
		int DesktopY { get; }

		/// <summary>
		/// Windows:
		/// String - "\\\\.\\DISPLAY3", etc. Can be used in EnumDisplaySettings/CreateDC.
		/// MacOS:
		/// int - DisplayId
		/// </summary>
		object DisplayDevice { get; }

        /// <summary>
        /// Device capabilities
        /// </summary>
        uint Caps { get; }

        /// <summary>
        /// Distortion capabilities for the HMD.
        /// </summary>
        uint DistortionCaps { get; }

        /// <summary>
        /// Default field of view per eye
        /// </summary>
        IFovPort[] DefaultEyeFov { get; }

        /// <summary>
        /// Maximum field of view per eye
        /// </summary>
        IFovPort[] MaxEyeFov { get; }

        /// <summary>
        /// Recommended eye rendering order, use for best performance.
        /// </summary>
        EyeType[] EyeRenderOrder { get; }
	}
}