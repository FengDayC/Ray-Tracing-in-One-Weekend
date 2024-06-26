#pragma once
#include<G3D/G3D.h>

namespace SoftRayTracing
{
	struct HitInfo;

	class Material : public ReferenceCountedObject
	{
	public:
		virtual bool scatter(const HitInfo& hitInfo, Ray& ray, Color3& attenuation) const = 0;
	};

	class Lambertian : public Material
	{
	public:
		virtual bool scatter(const HitInfo& hitInfo, Ray& ray, Color3& attenuation) const override;

	public:
		static ReferenceCountedPointer<Lambertian> create(const Color3& albedo);

		~Lambertian() = default;

	protected:
		Lambertian(const Color3& albedo);

		Color3 m_albedo;
	};

	class Metal : public Material
	{
	public:
		virtual bool scatter(const HitInfo& hitInfo, Ray& ray, Color3& attenuation) const override;

	public:
		static ReferenceCountedPointer<Metal> create(const Color3& albedo);

		~Metal() = default;

	protected:
		Metal(const Color3& albedo);

		Color3 m_albedo;
	};

	class Dielectric : public Material
	{
	public:
		virtual bool scatter(const HitInfo& hitInfo, Ray& ray, Color3& attenuation) const override;

	public:
		static ReferenceCountedPointer<Dielectric> create(float ir);

		~Dielectric() = default;

	protected:
		Dielectric(float ir);

		Vector3 refract(const Vector3& v, const Vector3& n, float niOverNt) const;

		// Use Schlick's approximation for reflectance.
		float reflectance(float cosine, float ref_idx) const;

		float m_ir;
	};

	static const ReferenceCountedPointer<Material> s_greyLambertian = dynamic_pointer_cast<Lambertian>(Lambertian::create(Color3::gray()));

	static const ReferenceCountedPointer<Material> s_orangeMetal = dynamic_pointer_cast<Metal>(Metal::create(Color3::orange()));

	static const ReferenceCountedPointer<Material> s_transparentGlass = dynamic_pointer_cast<Dielectric>(Dielectric::create(1.5f));
}