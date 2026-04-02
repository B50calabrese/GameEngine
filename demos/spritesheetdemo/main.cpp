#include <engine/graphics/renderer.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/graphics/texture.h>
#include <engine/util/asset_manager.h>

#include "../common/demo_utils.h"

class SpriteSheetApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    // Let's assume card_back.png is a 2x2 sprite sheet for demonstration.
    // The actual image is probably just one card, but we can treat it as a
    // sheet. We'll use the AssetManager to load it with the new format.

    // First, let's get the texture to know its size for a more realistic demo.
    auto tex = engine::graphics::Texture::Load("scifi/png/Tiles/Tile (2).png");
    if (tex) {
      sprite_sheet_ = engine::graphics::SpriteSheet::Create(tex, tex->width(), tex->height(), 1, 1);
    }

    time_ = 0.0;
  }

  void OnDemoUpdate(double delta_time_seconds) override {
    time_ += delta_time_seconds;

    if (sprite_sheet_) {
      // Draw the sprite in the center
      engine::graphics::Renderer::Get().DrawSprite(
          sprite_sheet_.get(), 0, {400.0f, 300.0f},
          {200.0f, 200.0f}, static_cast<float>(time_ * 45.0),
          {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
    }
  }

 private:
  std::shared_ptr<engine::graphics::SpriteSheet> sprite_sheet_;
  double time_;
};

int main(void) {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine_config.window_height = 600;
  engine_config.window_width = 800;
  return demos::common::DemoRunner::Run<SpriteSheetApp>(engine_config);
}
