#pragma once
#include<G3D/G3D.h>

namespace SoftRayTracing
{
	class Camera : public ReferenceCountedObject
	{
	public:
		virtual void generateRays(Array<Ray>& raysBuffer, uint32_t width, int rayPerPixel = 1) = 0;

	public:

		inline void SetPosition(const Vector3& vPosition)
		{
			position = vPosition; 
			RecalculateViewMatrix(); 
		}

		inline void SetRotation(const Quat& vRotation)
		{
			rotation = vRotation; 
			RecalculateViewMatrix(); 
		}

		inline void SetAspectRatio(float vAspectRatio)
		{
			aspectRatio = vAspectRatio; 
			RecalculateViewMatrix(); 
		}

		inline Vector3 GetPosition() const 
		{ 
			return position; 
		}

		inline Quat GetRotation() const
		{
			return rotation; 
		}

		inline Matrix4 GetViewMatrix() const
		{
			return viewMatrix;
		}


	protected:

		void RecalculateViewMatrix();

		Matrix4 viewMatrix;

		Vector3 position;
		Quat rotation;

		float aspectRatio;
		float nearPlane;
		float farPlane;
	};

	class PerspectiveCamera : public Camera
	{
	public:
		virtual void generateRays(Array<Ray>& raysBuffer, uint32_t width, int rayPerPixel) override;

		static ReferenceCountedPointer<PerspectiveCamera> create(Vector3 position = Vector3::zero(), Quat rotation = Quat::fromAxisAngleRotation(Vector3(0,1,0), 0.0f)
			, float aspectRatio = 16.0f/9.0f, float nearPlane = 1.0f, float farPlane = 1000.0f);

	protected:
		float fovx;

	protected:
		PerspectiveCamera(Vector3 position, Quat rotation, float aspectRatio, float nearPlane, float farPlane);
	};
}
