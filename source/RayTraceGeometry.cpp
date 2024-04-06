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

	
	Hittable::Hittable(Vector3 position, Quat rotation, Vector3 scale)
		: Transformable(position, rotation, scale)
	{
	}

	bool Sphere::hit(Ray ray, float ray_min, float ray_max, HitInfo& hitInfo) const
	{
		Vector3 oc = ray.origin() - position;
		float a = ray.direction().dot(ray.direction());
		float b = 2.0f * oc.dot(ray.direction());
		float c = oc.dot(oc) - radius * radius;
		float discriminant = b * b - 4 * a * c;
		if (discriminant < 0)
		{
			hitInfo = missInfo;
			return false;
		}
		else
		{
			hitInfo.t = (-b - sqrt(discriminant)) / (2.0f * a);
			hitInfo.point = ray_at(ray, hitInfo.t);
			hitInfo.normal = (hitInfo.point - position) / radius;
			return true;
		}
	}

	ReferenceCountedPointer<Sphere> Sphere::create(Vector3 center, float radius)
	{
		return createShared<Sphere>(center, radius);
	}

	Sphere::Sphere(Vector3 center, float radius)
		: Hittable(center, Quat(Vector3::zero(),1.0f), Vector3::one()), radius(radius)
	{
	}

}

