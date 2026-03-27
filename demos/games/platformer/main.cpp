#include <engine/scene/scene_manager.h>

#include "../../common/demo_utils.h"
#include "../../common/menu_scene.h"
#include "scenes.h"

class PlatformerApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    std::vector<demos::common::BaseMenuScene::MenuItem> items = {
        {"Level 1",
         []() {
           engine::SceneManager::Get().SetScene(
               std::make_unique<platformer::GameplayScene>("Gameplay", 1));
         }},
        {"Level 2",
         []() {
           engine::SceneManager::Get().SetScene(
               std::make_unique<platformer::GameplayScene>("Gameplay", 2));
         }},
        {"Quit", []() { engine::Engine::Shutdown(); }}};
    engine::SceneManager::Get().SetScene(
        std::make_unique<demos::common::BaseMenuScene>("PLATFORMER DEMO",
                                                       items));
  }
};

int main(void) {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  return demos::common::DemoRunner::Run<PlatformerApp>(config);
}
