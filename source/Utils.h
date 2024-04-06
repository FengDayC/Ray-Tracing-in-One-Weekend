#pragma once
#include<G3D/G3D.h>

namespace SoftRayTracing
{
	Vector3 uniformRandomUnit();

	Vector3 semisphereUniformRandomUnit(Vector3 normal);

	float Q_rsqrt(float number);

	float linearToGamma(float f);
	
	float gammaToLinear(float f);
	
	Color3 linearToGamma(Color3 color);
	
	Color3 gammaToLinear(Color3 color);
}
