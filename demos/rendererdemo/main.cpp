#include "application.h"
#include "engine.h"
#include "graphics/renderer.h"

class RendererApp : public engine::Application {
 public:
  void OnInit() override {}

  void OnShutdown() override {}

  void OnUpdate(double deltaTimeSeconds) override {
    engine::graphics::Renderer::Get().DrawRect(0.0f, 0.0f, 100.0f, 100.0f);
  }
};

/**
 * Demo showing how to do basic drawing commands in the engine.
 */
int main(void) {
  engine::EngineConfig engine_config;
  engine::Engine::Init(engine_config);
  RendererApp renderer_app;
  renderer_app.Run();
  return 0;
}