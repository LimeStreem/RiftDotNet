using System;
using SharpDX;

namespace RiftDotNet
{
    public interface IHMDDevice
        : IDevice
    {
        new IHMDInfo Info { get; }
        ISensorState GetSensorState(double absoluteTime);

        ISensorInfo SensorInfo { get; }

        IEyeRenderDesc GetEyeRenderSettings(IEyeDesc eye);

        ITextureSize GetFovTextureSize(EyeType eye, IFovPort fov, float pixelsPerDisplayPixel);

        IFrameTiming BeginFrameTiming(uint frameIndex);

        IPosef GetEyePose(EyeType eye);

        bool CreateDistortionMesh(IEyeDesc eyeDesc, uint distortionCaps,
            out Vector2[] uvScaleOffsetOut, out IDistortionMesh meshData);

        Matrix[] GetEyeTimewarpMatrices(EyeType eye, IPosef renderPose);

        Matrix CreateProjection(IFovPort fov, float znear, float zfar, bool rightHanded);

        void ResetSensor();

        void EndFrameTiming();

        /// <summary>
        /// Wait until the specified time.
        /// </summary>
        /// <param name="absoluteTime"></param>
        /// <returns></returns>
        double WaitTillTime(double absoluteTime);

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