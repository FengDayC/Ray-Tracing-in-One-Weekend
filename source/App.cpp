/** \file App.cpp */
#include "App.h"

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
        settings.window.height          = int(OSWindow::primaryDisplayWindowSize().y * 0.95f); 
        // Constrain ultra widescreen aspect ratios
        settings.window.width           = min(settings.window.height * 1920 / 1080, int(OSWindow::primaryDisplayWindowSize().x * 0.95f));

        // Make even
        settings.window.width  -= settings.window.width & 1;
        settings.window.height -= settings.window.height & 1;
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

    return App(settings).run();
}


App::App(const GApp::Settings& settings) : GApp(settings) {
}

void App::onInit() {
    GApp::onInit();

    setFrameDuration(1.0f / 240.0f);
    
    showRenderingStats      = true;

    makeGUI();
}


void App::makeGUI() {
}


void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& allSurfaces) {
}


void App::onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& posed2D) {
    Surface2D::sortAndRender(rd, posed2D);
}
