#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/graphics/texture.h>
#include <engine/util/asset_manager.h>

class SpriteSheetApp : public engine::Application {
 public:
  void OnInit() override {
    // Let's assume card_back.png is a 2x2 sprite sheet for demonstration.
    // The actual image is probably just one card, but we can treat it as a sheet.
    // We'll use the AssetManager to load it with the new format.

    // First, let's get the texture to know its size for a more realistic demo.
    auto tex = engine::graphics::Texture::Load("card_back.png");
    if (tex) {
        int sw = tex->width() / 2;
        int sh = tex->height() / 2;
        std::string load_path = "card_back.png:" + std::to_string(sw) + ":" +
                                std::to_string(sh) + ":2:2";
        sprite_sheet_ = engine::util::AssetManager<engine::graphics::SpriteSheet>::Get(load_path);
    }

    time_ = 0.0;
  }

  void OnShutdown() override {}

  void OnUpdate(double delta_time_seconds) override {
    time_ += delta_time_seconds;

    if (sprite_sheet_) {
        // Draw the 4 sprites in a grid
        float x_off = 100.0f;
        float y_off = 100.0f;
        float spacing = 200.0f;

        for (int i = 0; i < 4; ++i) {
            float x = x_off + (i % 2) * spacing;
            float y = y_off + (i / 2) * spacing;

            engine::graphics::Renderer::Get().DrawSprite(
                sprite_sheet_.get(), i, {x, y}, {150.0f, 150.0f});
        }

        // Draw one rotating in the center
        int animated_index = static_cast<int>(time_) % 4;
        engine::graphics::Renderer::Get().DrawSprite(
            sprite_sheet_.get(), animated_index, {400.0f, 300.0f}, {200.0f, 200.0f},
            static_cast<float>(time_ * 45.0), {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
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
  engine::Engine::Init(engine_config);
  SpriteSheetApp app;
  app.Run();
  return 0;
}
