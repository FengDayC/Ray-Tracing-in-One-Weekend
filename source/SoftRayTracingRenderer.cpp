#include "SoftRayTracingRenderer.h"
#include "RayTraceGeometry.h"
#include "Camera.h"

namespace SoftRayTracing
{
	SoftRayTracingRenderer::SoftRayTracingRenderer(int raysPerPixel)
		:raysPerPixel(raysPerPixel)
	{
	}
	void SoftRayTracingRenderer::render(RenderDevice* rd, ReferenceCountedPointer<SoftRayTracing::Camera> camera, Array<ReferenceCountedPointer<Hittable>>& objects)
	{
		if (!m_frameTexture || m_frameTexture->width() != rd->width() || m_frameTexture->height() != rd->height())
		{
			m_frameTexture = Texture::createEmpty("FrameTexture", rd->width(), rd->height(), ImageFormat::RGBA32F());
			camera->SetAspectRatio(rd->width() / (float)rd->height());
		}

		Array<Color4> frameBuffer;
		Array<Ray> raysBuffer;

		//Fill the frame buffer
		camera->generateRays(raysBuffer, rd->width(), raysPerPixel);

		float weightPerRay = 1.0f / raysPerPixel;
		int pixelCnt = raysBuffer.size() / raysPerPixel;
		for (int i = 0, baseRayID = 0; i < pixelCnt; i++, baseRayID+=raysPerPixel)
		{
			Color4 result = Color4(0.0f, 0.5f, (float)(rd->width() - i / rd->width()) / rd->height(), 1.0f);
			bool isHit = false;
			for (int j = 0; j < raysPerPixel; j++)
			{
				Ray ray = raysBuffer[baseRayID + j];
				HitInfo hitInfo;
				hit(ray, objects, hitInfo);
				if (hitInfo.t < inf())
				{
					if (!isHit)
					{
						isHit = true;
						result = Color3(hitInfo.normal * weightPerRay);
					}
					result = result.rgb() + Color3(hitInfo.normal * weightPerRay);
				}
			}
			frameBuffer.append(result);
			//Color4 uvColor = Color4(0.5f*(ray.direction().x + 1.0f), 0.5f*(ray.direction().y + 1.0f), 0.5f*(ray.direction().z + 1.0f), 1.0f);
			//frameBuffer.append(uvColor);
		}

		// Post-process
		const shared_ptr<PixelTransferBuffer>& ptb = CPUPixelTransferBuffer::fromData(rd->width(), rd->height(), ImageFormat::RGBA32F(), frameBuffer.getCArray());
		m_frameTexture->update(ptb);

		rd->push2D(); {
			Draw::rect2D(Rect2D::xywh(0, 0, rd->width(), rd->height()), rd, Color4(1.0f,1.0f,1.0f,1.0f), m_frameTexture);
		} rd->pop2D();

		frameBuffer.clear();
		raysBuffer.clear();
	}

	void SoftRayTracingRenderer::hit(const Ray& ray, Array<ReferenceCountedPointer<Hittable>>& objects, HitInfo& hitInfo) const
	{
		hitInfo = missInfo;
		for (auto object : objects)
		{
			HitInfo tempHitInfo;
			if (object->hit(ray, 0.0f, 1000.0f, tempHitInfo))
			{
				if (tempHitInfo.t < hitInfo.t)
				{
					hitInfo = tempHitInfo;
				}
			}
		}
	}
}

