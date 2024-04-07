#pragma once
#include<G3D/G3D.h>
#include "Material.h"

namespace SoftRayTracing
{
	Vector3 ray_at(Ray r, float t);

	struct HitInfo
	{
		float t;
		Vector3 normal;
		Vector3 point;
		bool frontFace;
		ReferenceCountedPointer<Material> material;
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

		void inverse_transform_hit(HitInfo& hitInfo) const;

	protected:
		Transformable(Vector3 position, Quat rotation, Vector3 scale);

		Vector3 m_position;

		Quat m_rotation;

		Vector3 m_scale;

		Matrix4 m_transformMatrix;

	private:

		void RecalculateTransformMatrix();
	};

	class Hittable : public ReferenceCountedObject, public Transformable
	{
	public:
		Hittable(Vector3 position, Quat rotation, Vector3 scale, ReferenceCountedPointer<Material> material);

		~Hittable() = default;

		virtual bool hit(Ray ray, float ray_min, float ray_max, HitInfo& hitInfo) const = 0;

	protected:
		ReferenceCountedPointer<Material> m_material;
	};

	class Sphere : public Hittable
	{
	private:
		float m_radius;

	public:

		
		virtual bool hit(Ray ray, float ray_min, float ray_max, HitInfo& hitInfo) const;
		

		static ReferenceCountedPointer<Sphere> create(Vector3 center = Vector3::zero(), float radius = 1.0f, ReferenceCountedPointer<Material> material = s_greyLambertian);
		

	protected:
		Sphere(Vector3 center, float radius, ReferenceCountedPointer<Material> material);
	};

	class Plane : public Hittable
	{
	public:
		
		virtual bool hit(Ray ray, float ray_min, float ray_max, HitInfo& hitInfo) const;
		

		/// <summary>
		/// create a plane
		/// </summary>
		/// <param name="position"></param>
		/// <param name="rotation"></param>
		/// <param name="size">x component infers the length of x axis, y component infers the length of z axis</param>
		/// <returns></returns>
		static ReferenceCountedPointer<Plane> create(Vector3 position = Vector3::zero(), 
			Quat rotation = Quat::fromAxisAngleRotation(Vector3(0.0f, 1.0f, 0.0f), 0.0f), Vector2 size = Vector2::one(), ReferenceCountedPointer<Material> material = s_greyLambertian);
		

	protected:
		Plane(Vector3 position, Quat rotation, Vector2 size, ReferenceCountedPointer<Material> material);

	};

}

