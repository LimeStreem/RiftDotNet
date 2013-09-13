﻿using System;

namespace RiftDotNet
{
	public interface ITestingFactory
		: IFactory
	{
		/// <summary>
		/// Creates an empty ISensorInfo instance.
		/// </summary>
		/// <returns></returns>
		ISensorInfo CreateSensorInfo();

		/// <summary>
		/// Creates an empty IHMDInfo instance.
		/// </summary>
		/// <returns></returns>
		IHMDInfo CreateHMDInfo();
	}
}