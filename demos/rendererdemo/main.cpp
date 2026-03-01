#include "application.h"
#include "engine.h"
#include "graphics/renderer.h"
#include "graphics/texture.h"

class RendererApp : public engine::Application {
 public:
  void OnInit() override {
    texture_ = engine::graphics::Texture::Create("card_back.png");
  }

  void OnShutdown() override {}

  void OnUpdate(double delta_time_seconds) override {
    engine::graphics::Renderer::Get().DrawRect(0.0f, 0.0f, 400.0f, 300.0f);
    if (texture_) {
        engine::graphics::Renderer::Get().DrawTexturedQuad(
            {400.0f, 300.0f}, {400.0f, 300.0f}, texture_.get());
    }
  }

 private:
  std::unique_ptr<engine::graphics::Texture> texture_;
};

/**
 * @brief Demo showing how to do basic drawing commands in the engine.
 */
int main(void) {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine_config.window_height = 600;
  engine_config.window_width = 800;
  engine::Engine::Init(engine_config);
  RendererApp renderer_app;
  renderer_app.Run();
  return 0;
}
