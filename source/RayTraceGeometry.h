#pragma once
#include<G3D/G3D.h>

namespace SoftRayTracing
{
	Vector3 ray_at(Ray r, float t);

	struct HitInfo
	{
		float t;
		Vector3 normal;
		Vector3 point;
	};

	const static HitInfo missInfo = { inf(), Vector3::zero(), Vector3::zero()};

	class Transformable
	{
	public:
		~Transformable() = default;

		Vector3 getPosition() const;

		Quat getRotation() const;

		Vector3 getScale() const;

		void setPosition(Vector3 vposition);

		void setRotation(Quat vrotation);

		void setScale(Vector3 vscale);

		Matrix4 getTransformMatrix() const;

		void transform_ray(Ray& ray) const;

	protected:
		Transformable(Vector3 position, Quat rotation, Vector3 scale);

		Vector3 position;

		Quat rotation;

		Vector3 scale;

		Matrix4 transformMatrix;

	private:

		void RecalculateTransformMatrix();
	};

	class Hittable : public ReferenceCountedObject, public Transformable
	{
	public:
		Hittable(Vector3 position, Quat rotation, Vector3 scale);

		~Hittable() = default;

		virtual bool hit(Ray ray, float ray_min, float ray_max, HitInfo& hitInfo) const = 0;

	};

	class Sphere : public Hittable
	{
	private:
		float radius;

	public:

		
		virtual bool hit(Ray ray, float ray_min, float ray_max, HitInfo& hitInfo) const;
		

		static ReferenceCountedPointer<Sphere> create(Vector3 center, float radius);
		

	protected:
		Sphere(Vector3 center, float radius);
	};

}

