using System;
using SharpDX;

namespace RiftDotNet
{
	/// <summary>
	/// Represents a head mounted display.
	/// There will only ever be one instance *per* physical HMD. This instance
	/// remains valid, even when the physical HMD is detached and subsequently
	/// attached (as long as there is only one HMDManager).
	/// </summary>
	public interface IHMD
	{
		/// <summary>
		/// Information about this device.
		/// Can be used to find out which coordinates a window should be placed at, so
		/// that the content of the window is actually shown on the device's screen.
		/// </summary>
		IHMDInfo Info { get; }

		/// <summary>
		/// This event is emitted when the physical HMD has been attached
		/// to the computer (again). Orientation, acceleration and angular velocity
		/// will only be valid as long as the device is attached, afterwards they will
		/// return the last valid value.
		/// </summary>
		event Action<IHMD> Attached;

		/// <summary>
		/// This event is emitted when the physical HMD has been detached from
		/// the computer.
		/// </summary>
		event Action<IHMD> Detached;

		/// <summary>
		/// Whether or not the physical HMD is currently attached to the computer
		/// and recoginized by the software.
		/// </summary>
		bool IsAttached { get; }

        /// <summary>
        /// Gets the sensor reading based on the given absolute time.
        /// Provide a 0.0 value to get the most current reading.
        /// </summary>
        /// <param name="absoluteTime">Absolute frame time</param>
        /// <returns>Read sensor state from the HMD.</returns>
	    ISensorState GetSensorState(double absoluteTime);

        /// <summary>
        /// Get render settings for the given eye description.
        /// </summary>
        /// <param name="eye">Eye settings.</param>
        /// <returns>Render distortion settings for the given eye.</returns>
	    IEyeRenderDesc GetEyeRenderSettings(IEyeDesc eye);

        /// <summary>
        /// Get recommended field of view (FOV) texture size.
        /// </summary>
        /// <param name="eye">Eye to render</param>
        /// <param name="fov">Field of view seetings for given eye.
        /// Use lower values to increase performance.</param>
        /// <param name="pixelsPerDisplayPixel">Pixels per display pixel, defaul tis 1.0.</param>
        /// <returns></returns>
        ITextureSize GetFovTextureSize(EyeType eye, IFovPort fov, float pixelsPerDisplayPixel);
        
        /// <summary>
        /// Call at the begning of the frame on the render thead.
        /// </summary>
        /// <param name="frameIndex">Frame Index, use 0 if GetFrameTiming isn't being use.</param>
        /// <returns>Gets frame timing for the giving frame index.</returns>
	    IFrameTiming BeginFrameTiming(uint frameIndex);

        /// <summary>
        /// Gets the Pose for rendering the given eye.
        /// </summary>
        /// <param name="eye">The given eye.</param>
        /// <returns>Returns the predicted pose for the given eye.</returns>
	    IPosef GetEyePose(EyeType eye);

        /// <summary>
        /// Creates a distortion mesh based on the distortionCaps flag.
        /// </summary>
        /// <param name="eyeDesc">Eye settings</param>
        /// <param name="distortionCaps">Distortion settings set on DistortionCaps enum values.</param>
        /// <param name="uvScaleOffsetOut">UV scale offset values populated based on eye settings.</param>
        /// <param name="meshData">Distortion mesh data that was created.</param>
        /// <returns>True if distortion mesh was created successfully, false otherwise.</returns>
        bool CreateDistortionMesh(IEyeDesc eyeDesc, uint distortionCaps,
           out Vector2[] uvScaleOffsetOut, out IDistortionMesh meshData);

		///<summary>
		/// Resets the current orientation and acceleration.
		///</summary>
		void Reset();

        /// <summary>
        /// Computes time warp orientation matrices used for distortion mesh shader.
        /// </summary>
        /// <param name="eye">Eye to time warp.</param>
        /// <param name="renderPose">Render pose to warp.</param>
        /// <returns>Two time warp matrices, the first is the start and the second the end orientation.</returns>
        Matrix[] GetEyeTimewarpMatrices(EyeType eye, IPosef renderPose);

        /// <summary>
        /// Create rendering projection matrix for the given field of view port.
        /// </summary>
        /// <param name="fov">FOV port</param>
        /// <param name="znear">Z near clipping plane</param>
        /// <param name="zfar">Z far clipping plan</param>
        /// <param name="rightHanded">Flag indicating if this is for a right handed 3d coordinate system.</param>
        /// <returns>A projection matrix for rendering use.</returns>
        Matrix CreateProjection(IFovPort fov, float znear, float zfar, bool rightHanded);

        /// <summary>
        /// Must be called after game rendered frane and present/swap buffers + GPU sync.
        /// </summary>
	    void EndFrameTiming();

        /// <summary>
        /// Wait until the specified time.
        /// </summary>
        /// <param name="absoluteTime">Absolute frame time.</param>
        /// <returns></returns>
        double WaitTillTime(double absoluteTime);

        /// <summary>
        /// Gets eye render settings for the given criteria.
        /// </summary>
        /// <param name="hmdCaps">Capabilities of the HMD.</param>
        /// <param name="distortionCaps">Distortion capabilities needed.</param>
        /// <param name="window">The render window.</param>
        /// <param name="eyeDescIn">Eye settings fo rendering.</param>
        /// <param name="eyeRenderDescOut">Generated eye render settings.</param>
        /// <returns>True if the eye render settings were set correctly, false otherwsie.</returns>
	    bool ConfigureOpenGlRendering(
	        uint hmdCaps,
	        uint distortionCaps,
	        IntPtr window,
	        IEyeDesc[] eyeDescIn,
	        out IEyeRenderDesc[] eyeRenderDescOut);

        IFrameTiming BeginFrame(uint frameIndex);

        void EndFrame();

        IPosef BeginEyeRender(EyeType eye);

        void EndEyeRender(EyeType eye,
               IPosef renderPose, ITextureSize textureSize, Viewport renderViewport,
                uint textureId);

        void WaitUntilGpuIdleGL();
	}
}
