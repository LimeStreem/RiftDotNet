using System;
using System.Threading;
using SharpDX;

namespace RiftDotNet
{
    internal sealed class HMD
        : IHMD
    {
        private IHMDInfo _info;
        private readonly ReaderWriterLockSlim _lock;
        private DeviceResources _resources;

        private bool _resetOutstanding;

        public HMD(IHMDInfo info, ReaderWriterLockSlim @lock)
        {
            if (info == null)
                throw new ArgumentNullException();

            if (@lock == null)
                throw new ArgumentNullException();

            _info = info;
            _lock = @lock;
        }

        internal DeviceResources Resources
        {
            set
            {
                _lock.EnterWriteLock();
                try
                {
                    if (value == _resources)
                        return;

                    if (_resources != null)
                    {
                        Action<IHMD> fn = Detached;
                        if (fn != null)
                            fn(this);
                    }

                    _resources = value;

                    if (_resources != null)
                    {
                        if (_resetOutstanding)
                        {
                            //_resources.Fusion.Reset();
                            _resetOutstanding = false;
                        }

                        // It may be possible that a newly attached device gets
                        // a different port, display position, whatever...
                        _info = _resources.Info;

                        // This is even more interesting. The user may have changed
                        // setting like yaw multiplier and whatnot: We want to apply
                        // those settings again.
                        //_resources.Fusion.YawCorrection = _yawCorrection;
                        //_resources.Fusion.IsPredictionEnabled = _isPredictionEnabled;
                        //_resources.Fusion.PredictionTime = _predictionTime;
                        //_resources.Fusion.AccelGain = _accelGain;

                        Action<IHMD> fn = Attached;
                        if (fn != null)
                            fn(this);
                    }
                }
                finally
                {
                    _lock.ExitWriteLock();
                }
            }
        }

        public IEyeRenderDesc GetEyeRenderSettings(IEyeDesc eye)
        {
            return _resources.Device.GetEyeRenderSettings(eye);
        }

        #region IHMD Members

        public IHMDInfo Info
        {
            get { return _info; }
        }

        public event Action<IHMD> Attached;
        public event Action<IHMD> Detached;

        public bool IsAttached
        {
            get { return _resources != null; }
        }

        public ISensorState GetSensorState(double absoluteTime)
        {
            return _resources.Device.GetSensorState(absoluteTime);
        }

        public void Reset()
        {
            _lock.EnterWriteLock();
            try
            {
                if (IsAttached)
                {
                    _resources.Device.ResetSensor();
                }
                else
                {
                    _resetOutstanding = true;
                }
            }
            finally
            {
                _lock.EnterWriteLock();
            }
        }

        public ITextureSize GetFovTextureSize(EyeType eye, IFovPort fov, float pixelsPerDisplayPixel)
        {
            return _resources.Device.GetFovTextureSize(eye, fov, pixelsPerDisplayPixel);
        }

        public IFrameTiming BeginFrameTiming(uint frameIndex)
        {
            return _resources.Device.BeginFrameTiming(frameIndex);
        }

        public IPosef GetEyePose(EyeType eye)
        {
            return _resources.Device.GetEyePose(eye);
        }

        public bool CreateDistortionMesh(IEyeDesc eyeDesc, uint distortionCaps,
           out Vector2[] uvScaleOffsetOut, out IDistortionMesh meshData)
        {
            return _resources.Device.CreateDistortionMesh(eyeDesc, distortionCaps,
                out uvScaleOffsetOut, out meshData);
        }

        public Matrix[] GetEyeTimewarpMatrices(EyeType eye, IPosef renderPose)
        {
            return _resources.Device.GetEyeTimewarpMatrices(eye, renderPose);
        }

        public Matrix CreateProjection(IFovPort fov, float znear, float zfar, bool rightHanded)
        {
            return _resources.Device.CreateProjection(fov, znear, zfar, rightHanded);
        }

        public void EndFrameTiming()
        {
            _resources.Device.EndFrameTiming();
        }

        public double WaitTillTime(double absoluteTime)
        {
            return _resources.Device.WaitTillTime(absoluteTime);
        }

        public bool ConfigureOpenGlRendering(
            uint hmdCaps,
            uint distortionCaps,
            IntPtr window,
            IEyeDesc[] eyeDescIn,
            out IEyeRenderDesc[] eyeRenderDescOut)
        {
            return _resources.Device.ConfigureOpenGlRendering(hmdCaps, distortionCaps,
                window, eyeDescIn, out eyeRenderDescOut);
        }

        public IFrameTiming BeginFrame(uint frameIndex)
        {
            return _resources.Device.BeginFrame(frameIndex);
        }

        public void EndFrame()
        {
            _resources.Device.EndFrame();
        }

        public IPosef BeginEyeRender(EyeType eye)
        {
            return _resources.Device.BeginEyeRender(eye);
        }

        public void EndEyeRender(EyeType eye,
            IPosef renderPose, ITextureSize textureSize, Viewport renderViewport,
            uint textureId)
        {
            _resources.Device.EndEyeRender(eye, renderPose, textureSize,
                renderViewport, textureId);
        }

        public void WaitUntilGpuIdleGL()
        {
            _resources.Device.WaitUntilGpuIdleGL();
        }

        #endregion
    }
}