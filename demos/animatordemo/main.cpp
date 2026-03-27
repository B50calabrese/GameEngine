#include <engine/graphics/renderer.h>
#include <engine/graphics/sprite_animator.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/graphics/texture.h>
#include <engine/input/input_manager.h>
#include <engine/util/asset_manager.h>
#include <engine/util/logger.h>

#include "../common/demo_utils.h"

class AnimatorDemoApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    // Load a sprite sheet. We'll use card_back.png as a 2x2 grid.
    auto tex = engine::graphics::Texture::Load("card_back.png");
    if (tex) {
      int sw = tex->width() / 2;
      int sh = tex->height() / 2;
      std::string load_path = "card_back.png:" + std::to_string(sw) + ":" +
                              std::to_string(sh) + ":2:2";
      sprite_sheet_ =
          engine::util::AssetManager<engine::graphics::SpriteSheet>::Get(
              load_path);
    }

    if (!sprite_sheet_) {
      LOG_ERR("Failed to load sprite sheet for demo.");
      return;
    }

    // Register animations
    engine::graphics::AnimationClip loop_clip;
    loop_clip.frames = {0, 1, 3, 2};
    loop_clip.fps = 4.0f;
    loop_clip.loop = true;
    engine::graphics::AnimationManager::Get().AddClip("loop", loop_clip);

    engine::graphics::AnimationClip once_clip;
    once_clip.frames = {0, 1, 2, 3};
    once_clip.fps = 2.0f;
    once_clip.loop = false;
    engine::graphics::AnimationManager::Get().AddClip("once", once_clip);

    // Initialize animators
    loop_animator_.Play("loop");
    once_animator_.Play("once");
  }

  void OnDemoUpdate(double delta_time_seconds) override {
    float dt = static_cast<float>(delta_time_seconds);

    // Input handling to restart oneshot
    if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::kSpace)) {
      if (once_animator_.IsFinished()) {
        once_animator_.Play("once");
      }
    }

    loop_animator_.Update(dt);
    once_animator_.Update(dt);

    if (sprite_sheet_) {
      // Draw looping animation
      engine::graphics::Renderer::Get().DrawText(
          "default", "Looping Animation (4 FPS)", {100.0f, 450.0f}, 0.0f, 0.5f);
      engine::graphics::Renderer::Get().DrawSprite(
          sprite_sheet_.get(), loop_animator_.GetCurrentFrame(),
          {150.0f, 250.0f}, {200.0f, 200.0f});

      // Draw oneshot animation
      std::string once_label = "Oneshot Animation (2 FPS)";
      if (once_animator_.IsFinished()) {
        once_label += " - FINISHED";
      }
      engine::graphics::Renderer::Get().DrawText("default", once_label,
                                                 {450.0f, 450.0f}, 0.0f, 0.5f);
      engine::graphics::Renderer::Get().DrawSprite(
          sprite_sheet_.get(), once_animator_.GetCurrentFrame(),
          {500.0f, 250.0f}, {200.0f, 200.0f});

      engine::graphics::Renderer::Get().DrawText(
          "default", "Press SPACE to restart oneshot", {100.0f, 100.0f}, 0.0f,
          0.5f);
    }
  }

 private:
  std::shared_ptr<engine::graphics::SpriteSheet> sprite_sheet_;
  engine::graphics::SpriteAnimator loop_animator_;
  engine::graphics::SpriteAnimator once_animator_;
};

int main(void) {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine_config.window_height = 600;
  engine_config.window_width = 800;
  return demos::common::DemoRunner::Run<AnimatorDemoApp>(engine_config);
}
