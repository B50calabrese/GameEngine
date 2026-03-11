#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/graphics/render_queue.h>

/**
 * @brief Demo showing the RenderQueue integrated into the Application
 * lifecycle.
 */
class RenderQueueDemo : public engine::Application {
 public:
  void OnInit() override {}

  void OnShutdown() override {}

  void OnUpdate(double delta_time_seconds) override {
    auto& queue = engine::graphics::RenderQueue::Default();

    // 1. Submit UI (High Z-order) - SUBMITTED FIRST
    // Even though it's submitted first, it will be rendered last due to
    // Z-order.
    queue.Submit({
        10.0f,
        0,  // Z-order 10
        {100.0f, 600.0f},
        {300.0f, 100.0f},
        {0.2f, 0.2f, 0.2f, 0.8f}  // Gray UI Panel
    });

    // 2. Submit World Background (Low Z-order) - SUBMITTED SECOND
    // Even though it's submitted second, it will be rendered first due to
    // Z-order.
    queue.Submit({
        -1.0f,
        0,  // Z-order -1
        {0.0f, 0.0f},
        {1280.0f, 720.0f},
        {0.1f, 0.4f, 0.1f, 1.0f}  // Green Grass
    });
  }
};

int main() {
  engine::EngineConfig config;
  config.window_title = "Render Queue Demo";
  engine::Engine::Init(config);

  RenderQueueDemo demo;
  demo.Run();

  engine::Engine::Shutdown();
  return 0;
}
