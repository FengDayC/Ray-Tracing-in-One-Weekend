#pragma once
#include "RayTraceGeometry.h"

namespace SoftRayTracing
{
	Vector3 ray_at(Ray r, float t)
	{
		return r.origin() + t * r.direction();
	}

	Transformable::Transformable(Vector3 position, Quat rotation, Vector3 scale)
		: m_position(position), m_rotation(rotation), m_scale(scale)
	{
		RecalculateTransformMatrix();
	}

	void Transformable::RecalculateTransformMatrix()
	{
		Matrix4 translation = Matrix4::translation(m_position);
		Matrix4 rotationMatrix = Matrix4(m_rotation.toRotationMatrix());
		Matrix4 scaleMatrix = Matrix4::scale(m_scale);
		m_transformMatrix = (translation * rotationMatrix * scaleMatrix).inverse();
	}

	Vector3 Transformable::getPosition() const
	{
		return m_position;
	}

	Quat Transformable::getRotation() const
	{
		return m_rotation;
	}

	Vector3 Transformable::getScale() const
	{
		return m_scale;
	}

	void Transformable::setPosition(Vector3 vposition)
	{
		m_position = vposition;
		RecalculateTransformMatrix();
	}

	void Transformable::setRotation(Quat vrotation)
	{
		m_rotation = vrotation;
		RecalculateTransformMatrix();
	}

	void Transformable::setScale(Vector3 vscale)
	{
		m_scale = vscale;
		RecalculateTransformMatrix();
	}

	Matrix4 Transformable::getTransformMatrix() const
	{
		return m_transformMatrix;
	}

	void Transformable::transform_ray(Ray& ray) const
	{
		//transform the ray
		Matrix4 rotationInverse = Matrix4(m_rotation.toRotationMatrix()).inverse();
		Vector4 rayOrigin = rotationInverse * Matrix4::translation(-m_position) * Vector4(ray.origin(), 1.0f);
		Vector4 rayDirection = rotationInverse * Vector4(ray.direction(), 0.0f);
		ray = Ray::fromOriginAndDirection(rayOrigin.xyz(), rayDirection.xyz(), ray.minDistance(), ray.maxDistance());
	}

	void Transformable::inverse_transform_hit(HitInfo& hitInfo) const
	{
		Matrix4 rotationMatrix = m_rotation.toRotationMatrix();
		hitInfo.point = m_position + (rotationMatrix * Vector4(hitInfo.point, 1.0f)).xyz();
		hitInfo.normal = (rotationMatrix * Vector4(hitInfo.normal, 0.0f)).xyz();
	}

	
	Hittable::Hittable(Vector3 position, Quat rotation, Vector3 scale, ReferenceCountedPointer<Material> material)
		: Transformable(position, rotation, scale), m_material(material)
	{
	}

	bool Sphere::hit(Ray ray, float ray_min, float ray_max, HitInfo& hitInfo) const
	{
		transform_ray(ray);
		float a = ray.direction().squaredMagnitude();
		float b = 2.0f * dot(ray.origin(), ray.direction());
		float c = ray.origin().squaredMagnitude() - m_radius * m_radius;
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
			hitInfo.normal = hitInfo.point / m_radius;
			hitInfo.material = m_material;
			inverse_transform_hit(hitInfo);
			return true;
		}
	}

	ReferenceCountedPointer<Sphere> Sphere::create(Vector3 center, float radius, ReferenceCountedPointer<Material> material)
	{
		return createShared<Sphere>(center, radius, material);
	}

	Sphere::Sphere(Vector3 center, float radius, ReferenceCountedPointer<Material> material)
		: Hittable(center, Quat(Vector3::zero(),1.0f), Vector3::one(), material), m_radius(radius)
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
			if (abs(point.x) < m_scale.x && abs(point.z) < m_scale.z)
			{
				hitInfo.t = t;
				hitInfo.point = point;
				hitInfo.normal = Vector3(0.0f, 1.0f, 0.0f);
				hitInfo.material = m_material;
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

	ReferenceCountedPointer<Plane> Plane::create(Vector3 position, Quat rotation, Vector2 size, ReferenceCountedPointer<Material> material)
	{
		return createShared<Plane>(position, rotation, size, material);
	}

	Plane::Plane(Vector3 position, Quat rotation, Vector2 size, ReferenceCountedPointer<Material> material)
		: Hittable(position, rotation, Vector3(size.x, 0.0f, size.y), material)
	{
	}
}

