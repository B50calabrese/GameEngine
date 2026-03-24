#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>

#include "main_menu_scene.h"

class TacticalRpgApp : public engine::Application {
 public:
  void OnInit() override {
    LOG_INFO("Tactical RPG Initialized");
    engine::SceneManager::Get().SetScene(
        std::make_unique<tactical_rpg::MainMenuScene>("MainMenu"));
  }
  void OnShutdown() override { LOG_INFO("Tactical RPG Shutdown"); }
  void OnUpdate(double dt) override {}
};

int main(int argc, char** argv) {
  engine::EngineConfig config;
  config.window_width = 1280;
  config.window_height = 720;
  config.window_title = "Tactical RPG Roguelike";
  config.asset_path = ENGINE_ASSETS_PATH;

  engine::Engine::Init(config);
  TacticalRpgApp app;
  app.Run();
  return 0;
}
