/** \file App.cpp */
#include "App.h"
#include "RayTraceGeometry.h"
#include "Camera.h"
#include "SoftRayTracingRenderer.h"
#include "Material.h"

// Tells C++ to invoke command-line main() function even on OS X and Win32.
G3D_START_AT_MAIN();

int main(int argc, const char* argv[]) {
    initGLG3D(G3DSpecification());

    GApp::Settings settings(argc, argv);

    // Change the window and other startup parameters by modifying the
    // settings class.  For example:
    settings.window.caption             = argv[0];

    // Set enable to catch more OpenGL errors
    // settings.window.debugContext     = true;
   
    settings.window.fullScreen          = false;
    if (settings.window.fullScreen) {
        settings.window.width           = 1920;
        settings.window.height          = 1080;
    } else {
        settings.window.width = 1024;
        settings.window.height = 768;
    }
    settings.window.resizable           = ! settings.window.fullScreen;
    settings.window.framed              = ! settings.window.fullScreen;
    settings.window.defaultIconFilename = "icon.png";

    // Set to true for a significant performance boost if your app can't
    // render at the display frequency, or if you *want* to render faster
    // than the display.
    settings.window.asynchronous        = true;

    // Render slightly larger than the screen so that screen-space refraction, bloom,
    // screen-space AO, and screen-space reflection to look good at screen edges. Set to zero for
    // maximum performance and free memory. Increase the second argument to improve AO without affecting
    // color. The third argument is the resolution. Set to 0.5f to render at half-res and upscale,
    // 2.0f to supersample.
    settings.hdrFramebuffer.setGuardBandsAndSampleRate(64, 0, 1.0f);

    settings.renderer.deferredShading = true;
    settings.renderer.orderIndependentTransparency = true;

    settings.dataDir                       = FileSystem::currentDirectory();

    settings.screenCapture.outputDirectory = FilePath::concat(FileSystem::currentDirectory(), "../journal");
    if (! FileSystem::exists(settings.screenCapture.outputDirectory)) {
        settings.screenCapture.outputDirectory = "";
    }
    settings.screenCapture.includeAppRevision = false;
    settings.screenCapture.includeG3DRevision = false;
    settings.screenCapture.filenamePrefix = "_";

    settings.useDeveloperTools = false;

    return App(settings).run();
}


App::App(const GApp::Settings& settings) : GApp(settings) {
}

void App::onInit() {
    GApp::onInit();

    setFrameDuration(1.0f / 240.0f);
    
    showRenderingStats      = true;

    m_softRayTracingRenderer = SoftRayTracing::SoftRayTracingRenderer::create(4, 16);
    m_camera = SoftRayTracing::PerspectiveCamera::create(Vector3(2.0f, 0, 1.5f), Quat::fromAxisAngleRotation(Vector3(0.0f, 1.0f, 0.0f), toRadians(30.0f)));

    m_sceneObjects.append(SoftRayTracing::Sphere::create(Vector3(0, 0, -3), 1.0f));
    m_sceneObjects.append(SoftRayTracing::Plane::create(Vector3(0, -1.0f, -3), Quat::fromAxisAngleRotation(Vector3(0, 1, 0), toRadians(0.0f)), Vector2::one() * 4.0f));
    m_sceneObjects.append(SoftRayTracing::Sphere::create(Vector3(-2, 0, -3), 1.0f, SoftRayTracing::s_orangeMetal));
    m_sceneObjects.append(SoftRayTracing::Sphere::create(Vector3(2, 0, -3), 1.0f, SoftRayTracing::s_transparentGlass));

    makeGUI();
}


void App::makeGUI() {
}


void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& allSurfaces) 
{
    GApp::onGraphics3D(rd, allSurfaces);

	m_softRayTracingRenderer->render(rd, m_camera, m_sceneObjects);
}


void App::onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& posed2D) {
    Surface2D::sortAndRender(rd, posed2D);
}
    

