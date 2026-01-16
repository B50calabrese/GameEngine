#include "application.h"
#include "engine.h"
#include "graphics/renderer.h"
#include "graphics/texture.h"

class RendererApp : public engine::Application {
 public:
  void OnInit() override {
    texture = engine::graphics::Texture::Create("card_back.png");
  }

  void OnShutdown() override {}

  void OnUpdate(double deltaTimeSeconds) override {
    engine::graphics::Renderer::Get().DrawRect(0.0f, 0.0f, 1.0f, 1.0f);
    engine::graphics::Renderer::Get().DrawTexturedRect(-1.0f, -1.0f, 1.0f, 1.0f,
                                                       texture->GetID());
  }

 private:
  engine::graphics::Texture* texture;
};

/**
 * Demo showing how to do basic drawing commands in the engine.
 */
int main(void) {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine::Engine::Init(engine_config);
  RendererApp renderer_app;
  renderer_app.Run();
  return 0;
}