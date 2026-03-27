#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>

#include "../../common/demo_utils.h"
#include "main_menu_scene.h"

class TacticalRpgApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    LOG_INFO("Tactical RPG Initialized");
    engine::SceneManager::Get().SetScene(
        std::make_unique<tactical_rpg::MainMenuScene>("MainMenu"));
  }
  void OnDemoShutdown() override { LOG_INFO("Tactical RPG Shutdown"); }
};

int main(int argc, char** argv) {
  engine::EngineConfig config;
  config.window_width = 1280;
  config.window_height = 720;
  config.window_title = "Tactical RPG Roguelike";
  config.asset_path = ENGINE_ASSETS_PATH;

  return demos::common::DemoRunner::Run<TacticalRpgApp>(config);
}
