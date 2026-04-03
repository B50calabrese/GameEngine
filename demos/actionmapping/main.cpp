#include <engine/input/action_manager.h>
#include <engine/util/logger.h>

#include "../common/demo_utils.h"

class ActionMappingApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    auto& am = engine::ActionManager::Get();
    am.BindAction("MoveRight", engine::KeyCode::kD);
    am.BindAction("MoveRight", engine::KeyCode::kRight);
    am.BindAction("Jump", engine::KeyCode::kSpace);

    LOG_INFO("ActionMapping Demo Initialized.");
    LOG_INFO("Hold 'D' or 'Right Arrow' to Move.");
    LOG_INFO("Press 'Space' to Jump.");
  }

  void OnDemoUpdate(double delta_time_seconds) override {
    auto& am = engine::ActionManager::Get();

    if (am.IsOngoing("MoveRight")) {
      object_pos_x_ += move_speed_ * static_cast<float>(delta_time_seconds);
      LOG_INFO("Moving Right... Position: {}", object_pos_x_);
    }

    if (am.IsStarted("Jump")) {
      LOG_INFO("Boing!");
    }

    if (am.IsReleased("Jump")) {
      LOG_INFO("Jump Released!");
    }
  }

 private:
  float object_pos_x_ = 0.0f;
  float move_speed_ = 100.0f;
};

int main(void) {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  engine_config.window_height = 600;
  engine_config.window_width = 800;
  engine_config.window_title = "Action Mapping Demo";

  return demos::common::DemoRunner::Run<ActionMappingApp>(engine_config);
}
