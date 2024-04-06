#include "Material.h"
#include "RayTraceGeometry.h"
#include "Utils.h"

namespace SoftRayTracing {
	bool Lambertian::scatter(const HitInfo& hitInfo, Ray& ray, Color3& attenuation) const
	{
		Vector3 direction = hitInfo.normal + uniformRandomUnit();
		if (direction.isZero())
		{
			direction = hitInfo.normal;
		}
		ray = Ray::fromOriginAndDirection(hitInfo.point, direction.direction());
		attenuation *= m_albedo;
		return true;
	}

	ReferenceCountedPointer<Lambertian> Lambertian::create(const Color3& albedo)
	{
		return createShared<Lambertian>(albedo);
	}

	Lambertian::Lambertian(const Color3& albedo)
		: m_albedo(albedo)
	{
	}

	bool Metal::scatter(const HitInfo& hitInfo, Ray& ray, Color3& attenuation) const
	{
		ray = ray.reflect(hitInfo.point, hitInfo.normal);
		attenuation *= m_albedo;
		return true;
	}

	ReferenceCountedPointer<Metal> Metal::create(const Color3& albedo)
	{
		return createShared<Metal>(albedo);
	}

	Metal::Metal(const Color3& albedo)
		: m_albedo(albedo)
	{
	}

	bool Dielectric::scatter(const HitInfo& hitInfo, Ray& ray, Color3& attenuation) const
	{
		ray = ray.refract(hitInfo.point, hitInfo.normal, m_ir, 1.0f);
		return true;
	}

	ReferenceCountedPointer<Dielectric> Dielectric::create(const Color3& albedo)
	{
		return ReferenceCountedPointer<Dielectric>();
	}

	Dielectric::Dielectric(float ir)
		: m_ir(ir)
	{
	}
}