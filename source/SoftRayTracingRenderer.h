#pragma once
#include<G3D/G3D.h>

namespace SoftRayTracing
{
	class Hittable;

	struct HitInfo;

	class Camera;

	class SoftRayTracingRenderer : public G3D::ReferenceCountedObject
	{	
	public:

		SoftRayTracingRenderer(int raysPerPixel, int maxBounceTime);

		void render(G3D::RenderDevice* rd, ReferenceCountedPointer<SoftRayTracing::Camera> camera, Array<ReferenceCountedPointer<Hittable>>& objects);

		void hit(const Ray& ray, HitInfo& hitInfo) const;

	protected:

		Color3 skyBox(Vector3 direction);

		Color3 shadeRay(Ray ray);

		ReferenceCountedPointer<G3D::Texture> m_frameTexture;

		Array<ReferenceCountedPointer<Hittable>> m_objectsCache;

	public:
		
		static ReferenceCountedPointer<SoftRayTracingRenderer> create(int raysPerPixel,int maxBounceTime)
		{
			return createShared<SoftRayTracingRenderer>(raysPerPixel, maxBounceTime);
		}

	private:
		int raysPerPixel;

		int maxBounceTime;
	};
}
