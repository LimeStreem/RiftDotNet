#pragma once

#include <OVR_SensorFusion.h>

#include "RiftDotNet.h"
#include "SensorDevice.h"

using namespace SharpDX;




namespace RiftDotNet
{
	namespace Platform
	{
		public ref class SensorFusion
			: public ISensorFusion
		{
		public:

			SensorFusion()
			{
				_native = new OVR::SensorFusion();
				
				IsPredictionEnabled = false;
			}

			SensorFusion(ISensorDevice^ device)
			{
				if (device != nullptr)
				{
					auto nativeSensor = ((SensorDevice^)device)->GetNative<OVR::SensorDevice>();
					if (nativeSensor == nullptr)
						throw gcnew ArgumentNullException("device", "The native pointer should not be null");

					_native = new OVR::SensorFusion(nativeSensor);
					reinterpret_cast<std::size_t>(_native),
					reinterpret_cast<std::size_t>(nativeSensor);
				}
				else
				{
					_native = new OVR::SensorFusion();
						reinterpret_cast<std::size_t>(_native);
				}

				_sensor = device;
			}

			!SensorFusion()
			{
				this->~SensorFusion();
			}

			~SensorFusion()
			{
				delete _native;
				_native = nullptr;
			}

			property bool IsDisposed
			{
				virtual bool get()
				{
					return _native == nullptr;
				}
			}

			property bool IsAttachedToSensor
			{
				virtual bool get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					return _native->IsAttachedToSensor();
				}
			}

			property bool IsGravityEnabled
			{
				virtual bool get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					return _native->IsGravityEnabled();
				}
				virtual void set(bool b)
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					_native->SetGravityEnabled(b);
				}
			}

			property ISensorDevice^ AttachedDevice
			{
				virtual ISensorDevice^ get()
				{
					return _sensor;
				}
				virtual void set(ISensorDevice^ value)
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					auto native = value != nullptr ? ((SensorDevice^)value)->GetNative<OVR::SensorDevice>() : nullptr;
					if (!_native->AttachToSensor(native))
					{
						throw gcnew System::InvalidOperationException("Attaching this sensor to the given device was not possible. Mabe the sensor already has a device attached to it? Please check the log");
					}

					_sensor = value;
				}
			}

			virtual void Reset()
			{
				if (IsDisposed)
					throw gcnew ObjectDisposedException("ISensorDevice");

				_native->Reset();
			}
		
			property Quaternion Orientation
			{
				virtual Quaternion get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					return FromNative(_native->GetOrientation());
				}
			}
		
			property Quaternion PredictedOrientation
			{
				virtual Quaternion get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					return FromNative(_native->GetPredictedOrientation());
				}
			}
		
			property Vector3 Acceleration
			{
				virtual Vector3 get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					return FromNative(_native->GetAcceleration());
				}
			}
		
			property Vector3 AngularVelocity
			{
				virtual Vector3 get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					return FromNative(_native->GetAngularVelocity());
				}
			}

			property float AccelGain
			{
				virtual float get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					return _native ->GetAccelGain();
				}
				virtual void set(float f)
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					_native->SetAccelGain(f);
				}
			}

			property bool YawCorrection
			{
				virtual bool get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					return _native->IsYawCorrectionEnabled();
				}
				virtual void set(bool value)
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					_native->SetYawCorrectionEnabled(value);
				}
			}
			
			property bool IsPredictionEnabled
			{
				virtual bool get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					// TODO: I want to be able to retrieve that value from OVR::SensorFusion
					return _predictionEnabled;
				}
				virtual void set(bool b)
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					_predictionEnabled = b;
					PredictionTime = PredictionTime;
				}
			}

			property TimeSpan PredictionTime
			{
				virtual TimeSpan get()
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					return TimeSpan::FromMilliseconds(_native->GetPredictionDelta());
				}

				virtual void set(TimeSpan value)
				{
					if (IsDisposed)
						throw gcnew ObjectDisposedException("ISensorDevice");

					_native->SetPrediction((float)value.TotalMilliseconds, _predictionEnabled);
				}
			}

		private:

			static Vector3 FromNative(const OVR::Vector3f& other)
			{
				Vector3 v;
				v.X = other.x;
				v.Y = other.y;
				v.Z = other.z;
				return v;
			}

			static Quaternion FromNative(const OVR::Quatf& other)
			{
				Quaternion quat;
				quat.X = other.x;
				quat.Y = other.y;
				quat.Z = other.z;
				quat.W = other.w;
				return quat;
			}

		private:

			OVR::SensorFusion* _native;
			ISensorDevice^ _sensor;
			bool _predictionEnabled;
		};
	}
}