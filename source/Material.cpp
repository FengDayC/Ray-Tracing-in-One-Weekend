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
		float refraction_ratio = hitInfo.frontFace ? (1.0f / m_ir) : m_ir;

		Vector3 n = hitInfo.frontFace ? hitInfo.normal : -hitInfo.normal;
		float cos_theta = dot(-ray.direction(), n);
		float sin_theta = sqrt(1.0f - square(cos_theta));

		bool cannot_refract = (refraction_ratio) > 1.0f;
		Vector3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > uniformRandom(0.0f, 1.0f))
		{
			direction = ray.direction().reflectionDirection(n);
		}
		else
		{
			direction = refract(ray.direction(), n, refraction_ratio);
		}
		ray = Ray::fromOriginAndDirection(hitInfo.point, direction);
		return true;
	}

	ReferenceCountedPointer<Dielectric> Dielectric::create(float ir)
	{
		return createShared<Dielectric>(ir);
	}

	Dielectric::Dielectric(float ir)
		: m_ir(ir)
	{
	}
	Vector3 Dielectric::refract(const Vector3& v, const Vector3& n, float niOverNt) const
	{
		auto cos_theta = fmin(dot(-v, n), 1.0f);
		Vector3 r_out_perp = niOverNt * (v + cos_theta * n);
		Vector3 r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.squaredMagnitude())) * n;
		return r_out_perp + r_out_parallel;
	}

	float Dielectric::reflectance(float cosine, float ref_idx) const
	{
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
}