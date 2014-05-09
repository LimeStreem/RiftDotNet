#pragma once

#include "RiftDotNet.h"
#include "HMDInfo.h"
#include "SensorState.h"
#include "SensorInfo.h"
#include "EyeRenderDesc.h"
#include "FrameTiming.h"
#include "Helper.h"
#include "DistortionMesh.h"

#include <OVR_CAPI_GL.h>
#include <CAPI\GL\CAPI_GL_DistortionRenderer.h>

namespace RiftDotNet
{
	namespace Platform
	{
		public ref class HMDDevice sealed
			: public IHMDDevice
		{
		private:
			ovrHmd _native;
			IHMDInfo^ _hmdInfo;
			IntPtr _equalityHandle;

		public:

			HMDDevice(const ovrHmd& native)
			{
				_native = native;

				// create sensor
				ovrBool success = ovrHmd_StartSensor(_native, ovrHmdCap_Orientation | ovrHmdCap_YawCorrection |
					ovrHmdCap_Position | ovrHmdCap_LowPersistence,
					ovrHmdCap_Orientation);

				if (!success){
					throw gcnew InvalidOperationException("Unable to create sensor for device!");
				}

				// create info for device
				ovrHmdDesc info;
				ovrHmd_GetDesc(_native, &info);
				_hmdInfo = gcnew RiftDotNet::Platform::HMDInfo(info);
			}

			property DeviceType Type
			{
				virtual DeviceType get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("IDevice");

					return DeviceType::HMD;
				}
			}

			property RiftDotNet::IDeviceInfo^ Info
			{
				virtual RiftDotNet::IDeviceInfo^ get() { return Info1; }
			}

			property RiftDotNet::IHMDInfo^ Info1
			{
				virtual RiftDotNet::IHMDInfo^ get() = RiftDotNet::IHMDDevice::Info::get
				{
				if (IsDisposed)
				throw gcnew ObjectDisposedException("IHMDDevice");

				return _hmdInfo;
			}
			}

			virtual ISensorState^ GetSensorState(double absoluteTime)
			{
				if (IsDisposed)
					throw gcnew ObjectDisposedException("IHMDDevice");

				ovrSensorState ss = ovrHmd_GetSensorState(_native, absoluteTime);
				return gcnew SensorSate(ss);
			}

			property ISensorInfo^ SensorInfo
			{
				virtual ISensorInfo^ get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("IHMDDevice");

					ovrSensorDesc sd;
					ovrHmd_GetSensorDesc(_native, &sd);
					return gcnew RiftDotNet::Platform::SensorInfo(sd);
				}
			}

			virtual bool Equals(Object^ other) override sealed
			{
				if (other == nullptr)
					return false;

				auto tmp = dynamic_cast<HMDDevice^>(other);
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
				return Equals((Object^) other);
			}

			property bool IsDisposed
			{
				virtual bool get()
				{
					return _native == nullptr;
				}
			}

			HMDDevice::~HMDDevice()
			{
			}

			virtual void ResetSensor()
			{
				ovrHmd_ResetSensor(_native);
			}

			virtual IEyeRenderDesc^ GetEyeRenderSettings(IEyeDesc^ eye)
			{
				ovrEyeDesc eyeDesc;
				eyeDesc.Fov.DownTan = eye->Fov->DownTan;
				eyeDesc.Fov.LeftTan = eye->Fov->LeftTan;
				eyeDesc.Fov.RightTan = eye->Fov->RightTan;
				eyeDesc.Fov.UpTan = eye->Fov->UpTan;
				eyeDesc.RenderViewport.Pos.x = eye->RenderViewport.X;
				eyeDesc.RenderViewport.Pos.y = eye->RenderViewport.Y;
				eyeDesc.RenderViewport.Size.h = eye->RenderViewport.Height;
				eyeDesc.RenderViewport.Size.w = eye->RenderViewport.Width;
				eyeDesc.TextureSize.h = eye->TextureSize->Height;
				eyeDesc.TextureSize.w = eye->TextureSize->Width;
				eyeDesc.Eye = (ovrEyeType) eye->Eye;

				ovrEyeRenderDesc renderDesc = ovrHmd_GetRenderDesc(_native, eyeDesc);
				return gcnew EyeRenderDesc(renderDesc);
			}

			virtual ITextureSize^ GetFovTextureSize(EyeType eye, IFovPort^ fov, float pixelsPerDisplayPixel)
			{
				ovrFovPort fovPort;
				fovPort.DownTan = fov->DownTan;
				fovPort.LeftTan = fov->LeftTan;
				fovPort.RightTan = fov->RightTan;
				fovPort.UpTan = fov->UpTan;
				ovrSizei textSize = ovrHmd_GetFovTextureSize(_native, (ovrEyeType) eye,
					fovPort, pixelsPerDisplayPixel);
				return gcnew TextureSize(textSize);
			}

			virtual IFrameTiming^ BeginFrameTiming(unsigned int frameIndex)
			{
				ovrFrameTiming frameTime = ovrHmd_BeginFrameTiming(_native, frameIndex);
				return gcnew FrameTiming(frameTime);
			}

			virtual IPosef^ GetEyePose(EyeType eye)
			{
				ovrPosef eyePose = ovrHmd_GetEyePose(_native, (ovrEyeType) eye);
				return gcnew Posef(eyePose);
			}

			virtual bool CreateDistortionMesh(IEyeDesc^ eyeDesc, unsigned int distortionCaps,
				array<Vector2>^% uvScaleOffsetOut, IDistortionMesh^% meshData)
			{
				ovrEyeDesc nEyeDesc = Helper::ToNative(eyeDesc);
				ovrVector2f uvScaleOffsets[2];
				ovrDistortionMesh mesh;
				ovrBool success = ovrHmd_CreateDistortionMesh(_native, nEyeDesc,
					distortionCaps, (ovrVector2f*) uvScaleOffsets, &mesh);

				for (size_t i = 0; i < 2; i++)
				{
					uvScaleOffsetOut[i] = Helper::FromNative(uvScaleOffsets[i]);
				}

				meshData = gcnew DistortionMesh(mesh);

				if (success)
					return true;
				else
					return false;
			}

			virtual array<Matrix>^ GetEyeTimewarpMatrices(EyeType eye, IPosef^ renderPose)
			{
				array<Matrix>^ timeWarps = gcnew array<Matrix>(2);
				ovrPosef pose;
				int indexCount = 2;
				ovrMatrix4f twm[2];
				ovrHmd_GetEyeTimewarpMatrices(_native, (ovrEyeType) eye, pose, twm);

				for (size_t i = 0; i < indexCount; i++)
				{
					timeWarps[0] = Helper::FromNative(twm[0]);
				}

				return timeWarps;
			}

			virtual Matrix CreateProjection(IFovPort^ fov, float znear, float zfar, bool rightHanded)
			{
				ovrFovPort fovPort = Helper::ToNative(fov);
				ovrMatrix4f projMat = ovrMatrix4f_Projection(fovPort, znear, zfar, rightHanded);
				Matrix proj = Helper::FromNative(projMat);
				return proj;
			}

			virtual void EndFrameTiming()
			{
				ovrHmd_EndFrameTiming(_native);
			}

			virtual double WaitTillTime(double absoluteTime)
			{
				return ovr_WaitTillTime(absoluteTime);
			}

			virtual bool ConfigureOpenGlRendering(
				unsigned int hmdCaps,
				unsigned int distortionCaps,
				System::IntPtr  window,
				array<IEyeDesc^>^ eyeDescIn,
				array<IEyeRenderDesc^>^% eyeRenderDescOut)
			{
				ovrGLConfig cfg;
				cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
				ovrSizei size;
				size.w = _hmdInfo->HResolution;
				size.h = _hmdInfo->VResolution;
				cfg.OGL.Header.RTSize = size;
				cfg.OGL.Header.Multisample = 1;
				cfg.OGL.Window = (HWND)window.ToPointer();
				cfg.OGL.WglContext = wglGetCurrentContext();
				cfg.OGL.GdiDc = wglGetCurrentDC();

				ovrEyeDesc eyes[2];
				ovrEyeRenderDesc eyRenderDescs[2];

				for (size_t i = 0; i < 2; i++)
				{
					eyes[i] = Helper::ToNative(eyeDescIn[i]);
				}

				ovrBool result = ovrHmd_ConfigureRendering(_native, &cfg.Config, hmdCaps, distortionCaps,
					eyes, eyRenderDescs);

				eyeRenderDescOut = gcnew array<IEyeRenderDesc^>(2);

				for (size_t i = 0; i < 2; i++)
				{
					eyeRenderDescOut[i] = gcnew EyeRenderDesc(eyRenderDescs[i]);
				}

				return result;
			}

			virtual IFrameTiming^ BeginFrame(unsigned int frameIndex)
			{
				ovrFrameTiming timing = ovrHmd_BeginFrame(_native, frameIndex);
				return gcnew FrameTiming(timing);
			}

			virtual void EndFrame()
			{
				ovrHmd_EndFrame(_native);
			}

			virtual IPosef^ BeginEyeRender(EyeType eye)
			{
				ovrPosef pose = ovrHmd_BeginEyeRender(_native, (ovrEyeType) eye);
				return gcnew Posef(pose);
			}

			virtual void  EndEyeRender(EyeType eye,
				IPosef^ renderPose, ITextureSize^ textureSize, Viewport renderViewport,
				unsigned int textureId)
			{
				ovrPosef pose = Helper::ToNative(renderPose);
				ovrGLTexture texture;
				texture.OGL.Header.API = ovrRenderAPI_OpenGL;
				texture.OGL.Header.TextureSize = Helper::ToNative(textureSize);
				texture.OGL.Header.RenderViewport = Helper::ToNative(renderViewport);
				//GLint texId;
				//glGetIntegerv(GL_TEXTURE_BINDING_2D, &texId);
				texture.OGL.TexId = (GLuint)textureId;
				ovrHmd_EndEyeRender(_native, (ovrEyeType) eye, pose, &texture.Texture);
			}

			virtual void WaitUntilGpuIdleGL()
			{
				glFlush();
				glFinish();
			}
		};
	}
}
