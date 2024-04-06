#include "Camera.h"

namespace SoftRayTracing
{
	void SoftRayTracing::Camera::RecalculateViewMatrix()
	{
		viewMatrix = Matrix4::translation(-position) * Matrix4(rotation.toRotationMatrix());
	}

	void PerspectiveCamera::generateRays(Array<Ray>& raysBuffer, uint32_t width, int rayPerPixel)
	{
		Matrix3 rotationMatrix = rotation.toRotationMatrix();
		Vector3 forward = -rotationMatrix.column(2);
		Vector3 right = rotationMatrix.column(0);
		Vector3 up = rotationMatrix.column(1);

		int height = width / aspectRatio;

		float rfovx = toRadians(fovx);
		Vector2 size = Vector2(tan(rfovx/2)*nearPlane, tan(rfovx/2)*nearPlane/aspectRatio);
		Vector2 deltaSize = Vector2(2.0f / width, 2.0f / height);
		for(int j = 0; j < height; j++)
		{
			float v = 2.0f * ((height - j + 0.5f) / height) - 1.0f;
			for (int i = 0; i < width; i++)
			{
				float u = 2.0f * ((i + 0.5f) / width) - 1.0f;
				if (rayPerPixel > 1)
				{
					for (int k = 0; k < rayPerPixel; k++)
					{
						float deltau = uniformRandom(-0.5f, 0.5f) * deltaSize.x;
						float deltav = uniformRandom(-0.5f, 0.5f) * deltaSize.y;
						Vector3 direction = forward * nearPlane + right * size.x * (u + deltau) + up * size.y * (v + deltav);
						raysBuffer.append(Ray::fromOriginAndDirection(position, direction.direction()));
					}
				}
				else
				{
					Vector3 direction = forward * nearPlane + right * size.x * u + up * size.y * v;
					raysBuffer.append(Ray::fromOriginAndDirection(position, direction.direction()));
				}
			}
		}
	}

	ReferenceCountedPointer<PerspectiveCamera> SoftRayTracing::PerspectiveCamera::create(Vector3 position, Quat rotation, float aspectRatio, float nearPlane, float farPlane)
	{
		return createShared<PerspectiveCamera>(position, rotation, aspectRatio, nearPlane, farPlane);
	}

	PerspectiveCamera::PerspectiveCamera(Vector3 position, Quat rotation, float aspectRatio, float nearPlane, float farPlane)
	{
		this->position = position;
		this->aspectRatio = aspectRatio;
		this->nearPlane = nearPlane;
		this->farPlane = farPlane;
		this->rotation = rotation;
		fovx = 90.0f;
	}
}

