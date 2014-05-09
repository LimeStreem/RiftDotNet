using System;

namespace RiftDotNet
{
	/// <summary>
	/// This structure uniquely identifies device instances.
	/// </summary>
	public struct DeviceKey : IEquatable<DeviceKey>
	{
        private readonly UInt16 _vendorId;
        private readonly UInt16 _productId;
		private readonly string _serialNumber;

		public DeviceKey(ISensorInfo deviceInfo)
		{
			_productId = deviceInfo.ProductId;
			_vendorId = deviceInfo.VendorId;
			_serialNumber = deviceInfo.SerialNumber;
		}

		#region IEquatable<DeviceKey> Members

		public bool Equals(DeviceKey other)
		{
            return _vendorId == other._vendorId &&
                   string.Equals(_productId, other._productId) &&
                   string.Equals(_serialNumber, other._serialNumber);
		}

		#endregion

		public override bool Equals(object obj)
		{
			if (ReferenceEquals(null, obj)) return false;
			return obj is DeviceKey && Equals((DeviceKey)obj);
		}

		public override int GetHashCode()
		{
			unchecked
			{
				var hashCode = (int)_vendorId;
                hashCode = (hashCode * 397) ^ (_productId.GetHashCode());
				hashCode = (hashCode * 397) ^ (_serialNumber != null ? _serialNumber.GetHashCode() : 0);
				return hashCode;
			}
		}

		public static bool operator ==(DeviceKey left, DeviceKey right)
		{
			return left.Equals(right);
		}

		public static bool operator !=(DeviceKey left, DeviceKey right)
		{
			return !left.Equals(right);
		}
	}
}