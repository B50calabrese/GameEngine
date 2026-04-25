#include <algorithm>
#include <string>

#include <engine/graphics/renderer.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/graphics/text_renderer.h>
#include <engine/graphics/texture.h>
#include <engine/input/input_manager.h>
#include <engine/util/asset_manager.h>

#include "../common/demo_utils.h"

class SpriteSheetApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    // Load the demo sprite sheet
    // 1408 / 11 = 128px per frame width
    // 768 / 4 = 192px per frame height
    auto tex =
        engine::graphics::Texture::Load("textures/sprite_sheet_demo.png");
    if (tex) {
      sprite_sheet_ =
          engine::graphics::SpriteSheet::Create(tex, 140, 192, 4, 10);
      total_frames_ = 4 * 10;
    } else {
      total_frames_ = 0;
    }

    range_start_ = 0;
    range_end_ = 2;
    current_frame_ = 0;
    timer_ = 0.0;
    fps_ = 12.0;
  }

  void OnDemoUpdate(double delta_time_seconds) override {
    HandleInput();

    if (total_frames_ > 0) {
      // Update animation
      timer_ += delta_time_seconds;
      if (timer_ >= 1.0 / fps_) {
        timer_ = 0.0;
        current_frame_++;

        // Loop within range
        if (current_frame_ > range_end_ || current_frame_ < range_start_) {
          current_frame_ = range_start_;
        }
      }

      // Draw the current sprite in the center at native size
      if (sprite_sheet_) {
        engine::graphics::Renderer::Get().DrawSprite(
            sprite_sheet_.get(), current_frame_, {400.0f, 300.0f},
            {128.0f, 192.0f}, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
      }
    }

    // Render UI information
    RenderUI();
  }

 private:
  void HandleInput() {
    auto& input = engine::InputManager::Get();

    // W/S to change range start
    if (input.IsKeyPressed(engine::KeyCode::kW)) {
      range_start_ = std::min(range_start_ + 1, range_end_);
      if (current_frame_ < range_start_) current_frame_ = range_start_;
    }
    if (input.IsKeyPressed(engine::KeyCode::kS)) {
      range_start_ = std::max(range_start_ - 1, 0);
    }

    // A/D to change range end
    if (input.IsKeyPressed(engine::KeyCode::kD)) {
      range_end_ = std::min(range_end_ + 1, total_frames_ - 1);
    }
    if (input.IsKeyPressed(engine::KeyCode::kA)) {
      range_end_ = std::max(range_end_ - 1, range_start_);
      if (current_frame_ > range_end_) current_frame_ = range_start_;
    }
  }

  void RenderUI() {
    std::string info = "Frame: " + std::to_string(current_frame_) +
                       " | Range: [" + std::to_string(range_start_) + ", " +
                       std::to_string(range_end_) + "]";
    std::string controls = "Controls: W/S (Start), A/D (End)";

    auto& text_renderer = engine::graphics::TextRenderer::Get();
    text_renderer.DrawText("default", info, {10.0f, 580.0f}, 0.0f, 0.5f,
                           {1.0f, 1.0f, 1.0f, 1.0f});
    text_renderer.DrawText("default", controls, {10.0f, 550.0f}, 0.0f, 0.4f,
                           {0.8f, 0.8f, 0.8f, 1.0f});
  }

  std::shared_ptr<engine::graphics::SpriteSheet> sprite_sheet_;
  int total_frames_ = 0;
  int current_frame_ = 0;
  int range_start_ = 0;
  int range_end_ = 0;
  double timer_ = 0.0;
  double fps_ = 12.0;
};

int main(void) {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine_config.window_height = 600;
  engine_config.window_width = 800;
  return demos::common::DemoRunner::Run<SpriteSheetApp>(engine_config);
}
