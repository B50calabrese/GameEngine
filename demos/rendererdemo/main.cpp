#include <engine/graphics/renderer.h>
#include <engine/graphics/texture.h>

#include "../common/demo_utils.h"

class RendererApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    texture_ = engine::graphics::Texture::Load("card_back.png");
  }

  void OnDemoShutdown() override {}

  void OnDemoUpdate(double delta_time_seconds) override {
    engine::graphics::Renderer::Get().DrawRect(0.0f, 0.0f, 400.0f, 300.0f);
    if (texture_) {
      engine::graphics::Renderer::Get().DrawTexturedQuad(
          {400.0f, 300.0f}, {400.0f, 300.0f}, texture_.get());
    }
  }

 private:
  std::shared_ptr<engine::graphics::Texture> texture_;
};

/**
 * @brief Demo showing how to do basic drawing commands in the engine.
 */
int main(void) {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine_config.window_height = 600;
  engine_config.window_width = 800;
  return demos::common::DemoRunner::Run<RendererApp>(engine_config);
}
