#pragma once
#include<G3D/G3D.h>

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
}
