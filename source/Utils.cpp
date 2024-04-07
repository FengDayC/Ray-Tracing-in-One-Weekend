#pragma once
#include "Utils.h"

namespace SoftRayTracing
{
	Vector3 uniformRandomUnit()
	{
		float theta = uniformRandom(0, 2 * pi());
		float phi = uniformRandom(0, pi() / 2);
		return Vector3(cos(phi) * cos(theta), cos(phi) * sin(theta), sin(phi));
	}

	Vector3 semisphereUniformRandomUnit(Vector3 normal)
	{
		Vector3 result = uniformRandomUnit();
		if (dot(result, normal) > 0.0f)
		{
			return result;
		}
		else
		{
			return -result;
		}
	}

	float Q_rsqrt(float number) {
		long i;
		float x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *(long*)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y));

		return y;
	}

	float linearToGamma(float f)
	{
		return 1.0f / Q_rsqrt(f);
	}

	float gammaToLinear(float f)
	{
		return pow(f, 2.0f);
	}

	Color3 linearToGamma(Color3 color)
	{
		return Color3(linearToGamma(color.r), linearToGamma(color.g), linearToGamma(color.b));
	}

	Color3 gammaToLinear(Color3 color)
	{
		return Color3(gammaToLinear(color.r), gammaToLinear(color.g), gammaToLinear(color.b));
	}


}
