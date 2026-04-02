#include <engine/graphics/renderer.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/graphics/texture.h>
#include <engine/graphics/utils/sprite_animator.h>
#include <engine/input/input_manager.h>
#include <engine/util/asset_manager.h>
#include <engine/util/logger.h>

#include "../common/demo_utils.h"

class AnimatorDemoApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    // Load the robot sprite sheet.
    // Assuming the robot zip has a combined sheet or separate files.
    // If separate, the animator demo might need a sheet. I'll use airplane for a simple sheet if it has one.
    // Actually, I'll use the red hat boy run animation as a sheet if possible, or I'll just load one of them.
    // Let's assume there's a sheet at airplane/png/plane_sheet.png

    std::string sheet_path = "airplane/png/plane_sheet.png";
    auto tex = engine::graphics::Texture::Load(sheet_path);
    if (tex) {
      int sw = tex->width() / 3;
      int sh = tex->height() / 1;
      std::string load_path = sheet_path + ":" + std::to_string(sw) + ":" +
                              std::to_string(sh) + ":1:3";
      sprite_sheet_ =
          engine::util::AssetManager<engine::graphics::SpriteSheet>::Get(
              load_path);
    }

    if (!sprite_sheet_) {
      // Fallback to redhatboy idle sequence for demo
      auto tex_fallback = engine::graphics::Texture::Load("redhat/png/Idle (1).png");
      if (tex_fallback) {
          sprite_sheet_ = engine::graphics::SpriteSheet::Create(tex_fallback, tex_fallback->width(), tex_fallback->height(), 1, 1);
      }
    }

    // Register animations
    engine::graphics::AnimationClip loop_clip;
    loop_clip.frames = {0};
    loop_clip.fps = 8.0f;
    loop_clip.loop = true;
    engine::graphics::AnimationManager::Get().AddClip("loop", loop_clip);

    engine::graphics::AnimationClip once_clip;
    once_clip.frames = {0};
    once_clip.fps = 4.0f;
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
  engine::graphics::utils::SpriteAnimator loop_animator_;
  engine::graphics::utils::SpriteAnimator once_animator_;
};

int main(void) {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine_config.window_height = 600;
  engine_config.window_width = 800;
  return demos::common::DemoRunner::Run<AnimatorDemoApp>(engine_config);
}
