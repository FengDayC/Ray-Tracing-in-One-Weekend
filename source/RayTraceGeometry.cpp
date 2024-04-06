#pragma once
#include "RayTraceGeometry.h"

namespace SoftRayTracing
{
	Vector3 ray_at(Ray r, float t)
	{
		return r.origin() + t * r.direction();
	}

	Transformable::Transformable(Vector3 position, Quat rotation, Vector3 scale)
		: position(position), rotation(rotation), scale(scale)
	{
		RecalculateTransformMatrix();
	}

	void Transformable::RecalculateTransformMatrix()
	{
		Matrix4 translation = Matrix4::translation(position);
		Matrix4 rotationMatrix = Matrix4(rotation.toRotationMatrix());
		Matrix4 scaleMatrix = Matrix4::scale(scale);
		transformMatrix = (translation * rotationMatrix * scaleMatrix).inverse();
	}

	Vector3 Transformable::getPosition() const
	{
		return position;
	}

	Quat Transformable::getRotation() const
	{
		return rotation;
	}

	Vector3 Transformable::getScale() const
	{
		return scale;
	}

	void Transformable::setPosition(Vector3 vposition)
	{
		position = vposition;
		RecalculateTransformMatrix();
	}

	void Transformable::setRotation(Quat vrotation)
	{
		rotation = vrotation;
		RecalculateTransformMatrix();
	}

	void Transformable::setScale(Vector3 vscale)
	{
		scale = vscale;
		RecalculateTransformMatrix();
	}

	Matrix4 Transformable::getTransformMatrix() const
	{
		return transformMatrix;
	}

	void Transformable::transform_ray(Ray& ray) const
	{
		//transform the ray
		Matrix4 rotationInverse = Matrix4(rotation.toRotationMatrix()).inverse();
		Vector4 rayOrigin = rotationInverse * Matrix4::translation(-position) * Vector4(ray.origin(), 1.0f);
		Vector4 rayDirection = rotationInverse * Vector4(ray.direction(), 0.0f);
		ray = Ray::fromOriginAndDirection(rayOrigin.xyz(), rayDirection.xyz(), ray.minDistance(), ray.maxDistance());
	}

	void Transformable::inverse_transform_hit(HitInfo& hitInfo) const
	{
		Matrix4 rotationMatrix = rotation.toRotationMatrix();
		hitInfo.point = position + (rotationMatrix * Vector4(hitInfo.point, 1.0f)).xyz();
		hitInfo.normal = (rotationMatrix * Vector4(hitInfo.normal, 0.0f)).xyz();
	}

	
	Hittable::Hittable(Vector3 position, Quat rotation, Vector3 scale, Color3 diffuse)
		: Transformable(position, rotation, scale), Shadable(diffuse)
	{
	}

	bool Sphere::hit(Ray ray, float ray_min, float ray_max, HitInfo& hitInfo) const
	{
		transform_ray(ray);
		float a = ray.direction().squaredMagnitude();
		float b = 2.0f * dot(ray.origin(), ray.direction());
		float c = ray.origin().squaredMagnitude() - radius * radius;
		float t = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
		if (t < ray_min || t > ray_max)
		{
			hitInfo = missInfo;
			return false;
		}
		else
		{
			hitInfo.t = t;
			hitInfo.point = ray_at(ray, t);
			hitInfo.normal = hitInfo.point / radius;
			hitInfo.diffuse = diffuse;
			inverse_transform_hit(hitInfo);
			return true;
		}
	}

	ReferenceCountedPointer<Sphere> Sphere::create(Vector3 center, float radius, Color3 diffuse)
	{
		return createShared<Sphere>(center, radius, diffuse);
	}

	Sphere::Sphere(Vector3 center, float radius, Color3 diffuse)
		: Hittable(center, Quat(Vector3::zero(),1.0f), Vector3::one(), diffuse), radius(radius)
	{
	}

	bool Plane::hit(Ray ray, float ray_min, float ray_max, HitInfo& hitInfo) const
	{
		transform_ray(ray);
		float t = - ray.origin().y / ray.direction().y;
		if (t < ray_min || t > ray_max)
		{
			hitInfo = missInfo;
			return false;
		}
		else
		{
			Vector3 point = ray_at(ray, t);
			if (abs(point.x) < scale.x && abs(point.z) < scale.z)
			{
				hitInfo.t = t;
				hitInfo.point = point;
				hitInfo.normal = Vector3(0.0f, 1.0f, 0.0f);
				hitInfo.diffuse = diffuse;
				inverse_transform_hit(hitInfo);
				return true;
			}
			else
			{
				hitInfo = missInfo;
				return false;
			}
		}
	}

	ReferenceCountedPointer<Plane> Plane::create(Vector3 position, Quat rotation, Vector2 size, Color3 diffuse)
	{
		return createShared<Plane>(position, rotation, size, diffuse);
	}

	Plane::Plane(Vector3 position, Quat rotation, Vector2 size, Color3 diffuse)
		: Hittable(position, rotation, Vector3(size.x, 0.0f, size.y), diffuse)
	{
	}

	Color3 Shadable::shade()
	{
		return Color3();
	}

	void Shadable::setDiffuse(Color3 diffuse)
	{
		this->diffuse = diffuse;
	}

	Color3 Shadable::getDiffuse() const
	{
		return diffuse;
	}

	Shadable::Shadable(Color3 diffuse)
		: diffuse(diffuse)
	{

	}

}

