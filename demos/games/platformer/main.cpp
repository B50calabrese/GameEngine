#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/scene/scene_manager.h>
#include "../../common/menu_scene.h"

#include "scenes.h"

class PlatformerApp : public engine::Application {
 public:
  void OnInit() override {
    std::vector<demos::common::BaseMenuScene::MenuItem> items = {
        {"Level 1", []() { engine::SceneManager::Get().SetScene(std::make_unique<platformer::GameplayScene>("Gameplay", 1)); }},
        {"Level 2", []() { engine::SceneManager::Get().SetScene(std::make_unique<platformer::GameplayScene>("Gameplay", 2)); }},
        {"Quit", []() { engine::Engine::Shutdown(); }}
    };
    engine::SceneManager::Get().SetScene(
        std::make_unique<demos::common::BaseMenuScene>("PLATFORMER DEMO", items));
  }
  void OnShutdown() override {}
  void OnUpdate(double dt) override {}
};

int main(void) {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  engine::Engine::Init(config);
  PlatformerApp app;
  app.Run();
  return 0;
}
