#include <iostream>

#include <GLFW/glfw3.h>  // For raw key codes if needed, though we use KeyCode

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/input/action_manager.h>
#include <engine/util/logger.h>

class ActionMappingApp : public engine::Application {
 public:
  void OnInit() override {
    auto& am = engine::ActionManager::Get();
    am.BindAction("MoveRight", engine::KeyCode::kD);
    am.BindAction("MoveRight", engine::KeyCode::kRight);
    am.BindAction("Jump", engine::KeyCode::kSpace);

    LOG_INFO("ActionMapping Demo Initialized.");
    LOG_INFO("Hold 'D' or 'Right Arrow' to Move.");
    LOG_INFO("Press 'Space' to Jump.");
  }

  void OnShutdown() override {}

  void OnUpdate(double delta_time_seconds) override {
    auto& am = engine::ActionManager::Get();

    if (am.IsOngoing("MoveRight")) {
      object_pos_x_ += move_speed_ * static_cast<float>(delta_time_seconds);
      LOG_INFO("Moving Right... Position: %f", object_pos_x_);
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

  engine::Engine::Init(engine_config);

  ActionMappingApp app;
  app.Run();

  return 0;
}
