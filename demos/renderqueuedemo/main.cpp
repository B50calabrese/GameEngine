#include "engine.h"
#include "graphics/camera.h"
#include "graphics/renderer.h"
#include "util/render_queue.h"

int main() {
  engine::EngineConfig config;
  config.window_title = "Render Queue Demo";
  engine::Engine::Init(config);

  engine::graphics::Camera camera(0.0f, 1280.0f, 0.0f, 720.0f);
  engine::util::RenderQueue queue;

  while (!engine::Engine::window().ShouldClose()) {
    engine::graphics::Renderer::Get().Clear();
    engine::graphics::Renderer::Get().BeginFrame(camera);

    // 1. Submit UI (High Z-order) - SUBMITTED FIRST
    queue.Submit({
        10.0f,
        0,                 // Z-order 10
        {100, 600},        {300, 100},
        {0.2f, 0.2f, 0.2f, 0.8f}  // Gray UI Panel
    });

    // 2. Submit World Background (Low Z-order) - SUBMITTED SECOND
    queue.Submit({
        -1.0f,
        0,                 // Z-order -1
        {0, 0},            {1280, 720},
        {0.1f, 0.4f, 0.1f, 1.0f}  // Green Grass
    });

    // 3. Process the queue
    queue.Flush();
    queue.Clear();

    engine::graphics::Renderer::Get().EndFrame();
    engine::Engine::window().SwapBuffers();
    engine::Engine::window().PollEvents();
  }

  engine::Engine::Shutdown();
  return 0;
}
