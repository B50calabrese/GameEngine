#include <engine/scene/scene_manager.h>

#include "../../common/demo_utils.h"
#include "../../common/menu_scene.h"
#include "scenes.h"

class RpgApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    std::vector<demos::common::BaseMenuScene::MenuItem> items = {
        {"New Run",
         []() {
           rpg::GameState::Get().ResetRun();
           engine::SceneManager::Get().SetScene(
               std::make_unique<rpg::MapScene>("Map"));
         }},
        {"Quit", []() { engine::Engine::Shutdown(); }}};
    engine::SceneManager::Get().SetScene(
        std::make_unique<demos::common::BaseMenuScene>("RPG ROGUELIKE DEMO",
                                                       items));
  }
};

int main(void) {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  return demos::common::DemoRunner::Run<RpgApp>(config);
}
