#include "SoftRayTracingRenderer.h"
#include "RayTraceGeometry.h"
#include "Camera.h"
#include "Utils.h"

namespace SoftRayTracing
{
	SoftRayTracingRenderer::SoftRayTracingRenderer(int raysPerPixel, int maxBounceTime)
		:raysPerPixel(raysPerPixel), maxBounceTime(maxBounceTime)
	{
	}
	void SoftRayTracingRenderer::render(RenderDevice* rd, ReferenceCountedPointer<SoftRayTracing::Camera> camera, Array<ReferenceCountedPointer<Hittable>>& objects)
	{
		m_objectsCache = objects;

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
			Color4 result;
			bool isHit = false;
			for (int j = 0; j < raysPerPixel; j++)
			{
				Ray ray = raysBuffer[baseRayID + j];
				result = shadeRay(ray) * weightPerRay;
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

	void SoftRayTracingRenderer::hit(const Ray& ray, HitInfo& hitInfo) const
	{
		hitInfo = missInfo;
		for (auto object : m_objectsCache)
		{
			HitInfo tempHitInfo;
			if (object->hit(ray, 0.00000001f, 1000.0f, tempHitInfo))
			{
				if (tempHitInfo.t < hitInfo.t)
				{
					hitInfo = tempHitInfo;
				}
			}
		}
	}

	Color3 SoftRayTracingRenderer::skyBox(Vector3 direction)
	{
		return lerp(Color3(1.0, 1.0f, 1.0), Color3(0.5, 0.7, 1.0), 0.5f * direction.y + 0.5f);
	}

	Color4 SoftRayTracingRenderer::shadeRay(Ray ray)
	{
		Color3 weight = Color3::one();
		Color3 result = Color3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < maxBounceTime; i++)
		{
			HitInfo hitInfo;
			hit(ray, hitInfo);
			if (hitInfo.t < inf())
			{
				weight = weight * hitInfo.diffuse;
				//ray = Ray::fromOriginAndDirection(hitInfo.point, semisphereUniformRandomUnit(hitInfo.normal));
				ray = Ray::fromOriginAndDirection(hitInfo.point, (hitInfo.normal + uniformRandomUnit()).direction());
			}
			else
			{
				result = skyBox(ray.direction()) * weight;
				break;
			}
		}
		result *= weight;
		
		return Color4(result, 1.0f);
	}
}

