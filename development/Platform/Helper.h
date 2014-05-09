#pragma once

#include "RiftDotNet.h"

using namespace System;
using namespace System::ComponentModel;




namespace RiftDotNet
{
	namespace Platform
	{
		ref class Helper
		{
		public:
			static SharpDX::Vector3 FromNative(const ovrVector3f& other)
			{
				SharpDX::Vector3 v;
				v.X = other.x;
				v.Y = other.y;
				v.Z = other.z;
				return v;
			}

			static SharpDX::Vector2 FromNative(const ovrVector2f& other)
			{
				SharpDX::Vector2 v;
				v.X = other.x;
				v.Y = other.y;
				return v;
			}

			static SharpDX::Viewport FromNative(const ovrRecti& other)
			{
				return SharpDX::Viewport(other.Pos.x, other.Pos.y,
					other.Size.w, other.Size.h);
			}

			static SharpDX::Quaternion FromNative(const ovrQuatf& other)
			{
				SharpDX::Quaternion quat;
				quat.X = other.x;
				quat.Y = other.y;
				quat.Z = other.z;
				quat.W = other.w;
				return quat;
			}

			static SharpDX::Matrix FromNative(ovrMatrix4f matrix)
			{
				return SharpDX::Matrix(matrix.M[0][0],matrix.M[0][1], matrix.M[0][2], matrix.M[0][3],
									   matrix.M[1][0],matrix.M[1][1], matrix.M[1][2], matrix.M[1][3],
									   matrix.M[2][0],matrix.M[2][1], matrix.M[2][2], matrix.M[2][3],
									   matrix.M[3][0],matrix.M[3][1], matrix.M[3][2], matrix.M[3][3]);
			}

			static ovrEyeDesc ToNative(IEyeDesc^ eye)
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
				return eyeDesc;
			}

			static ovrPosef ToNative(IPosef^ pose)
			{
				ovrPosef nPose;
				nPose.Orientation.w = pose->Orientation.W;
				nPose.Orientation.x = pose->Orientation.X;
				nPose.Orientation.y = pose->Orientation.Y;
				nPose.Orientation.z = pose->Orientation.Z;
				nPose.Position.x = pose->Position.X;
				nPose.Position.y = pose->Position.Y;
				nPose.Position.z = pose->Position.Z;
				return nPose;
			}

			static ovrFovPort ToNative(IFovPort^ fov)
			{
				ovrFovPort fovPort;
				fovPort.DownTan = fov->DownTan;
				fovPort.LeftTan = fov->LeftTan;
				fovPort.RightTan = fov->RightTan;
				fovPort.UpTan = fov->UpTan;
				return fovPort;
			}

			static ovrSizei ToNative(ITextureSize^ textSize)
			{
				ovrSizei size;
				size.h = textSize->Height;
				size.w = textSize->Width;
				return size;
			}

			static ovrRecti ToNative(SharpDX::Viewport view)
			{
				ovrRecti vp;
				vp.Pos.x = view.X;
				vp.Pos.y = view.Y;
				vp.Size.h = view.Height;
				vp.Size.w = view.Width;
				return vp;
			}
		};
	}
}