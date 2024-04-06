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
		Color3 diffuse;
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

		Vector3 position;

		Quat rotation;

		Vector3 scale;

		Matrix4 transformMatrix;

	private:

		void RecalculateTransformMatrix();
	};

	class Shadable
	{
	public:

		~Shadable() = default;

		virtual Color3 shade();

		void setDiffuse(Color3 diffuse);

		Color3 getDiffuse() const;

	protected:

		Shadable(Color3 diffuse);

		Color3 diffuse;

	};

	class Hittable : public ReferenceCountedObject, public Transformable, public Shadable
	{
	public:
		Hittable(Vector3 position, Quat rotation, Vector3 scale, Color3 diffuse);

		~Hittable() = default;

		virtual bool hit(Ray ray, float ray_min, float ray_max, HitInfo& hitInfo) const = 0;


	};

	class Sphere : public Hittable
	{
	private:
		float radius;

	public:

		
		virtual bool hit(Ray ray, float ray_min, float ray_max, HitInfo& hitInfo) const;
		

		static ReferenceCountedPointer<Sphere> create(Vector3 center = Vector3::zero(), float radius = 1.0f, Color3 diffuse = Color3::gray());
		

	protected:
		Sphere(Vector3 center, float radius, Color3 diffuse);
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
			Quat rotation = Quat::fromAxisAngleRotation(Vector3(0.0f,1.0f,0.0f), 0.0f), Vector2 size = Vector2::one(), Color3 diffuse = Color3::gray());
		

	protected:
		Plane(Vector3 position, Quat rotation, Vector2 size, Color3 diffuse);

	};

}

