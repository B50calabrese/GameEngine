#include "application.h"
#include "engine.h"

class RendererApp : public engine::Application {
 public:
  void OnInit() override {}

  void OnShutdown() override {}

  void OnUpdate(double deltaTimeSeconds) override {}
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