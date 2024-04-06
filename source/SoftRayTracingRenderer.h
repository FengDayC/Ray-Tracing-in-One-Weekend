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

		SoftRayTracingRenderer(int raysPerPixel);

		void render(G3D::RenderDevice* rd, ReferenceCountedPointer<SoftRayTracing::Camera> camera, Array<ReferenceCountedPointer<Hittable>>& objects);

		void hit(const Ray& ray, Array<ReferenceCountedPointer<Hittable>>& objects, HitInfo& hitInfo) const;

	protected:

		ReferenceCountedPointer<G3D::Texture> m_frameTexture;

	public:
		
		static ReferenceCountedPointer<SoftRayTracingRenderer> create(int raysPerPixel)
		{
			return createShared<SoftRayTracingRenderer>(raysPerPixel);
		}

	private:
		int raysPerPixel;
	};
}
