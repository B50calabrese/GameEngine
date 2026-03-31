#include <engine/scene/scene_manager.h>

#include "../../common/demo_utils.h"
#include "../../common/menu_scene.h"
#include "gameplay_scene.h"

class BreakoutApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    std::vector<demos::common::BaseMenuScene::MenuItem> items = {
        {"Start Game",
         []() {
           engine::SceneManager::Get().SetScene(
               std::make_unique<breakout::GameplayScene>("Gameplay"));
         }},
        {"Quit", []() { engine::Engine::Shutdown(); }}};
    engine::SceneManager::Get().SetScene(
        std::make_unique<demos::common::BaseMenuScene>("BREAKOUT DEMO", items));
  }
};

int main(void) {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  return demos::common::DemoRunner::Run<BreakoutApp>(config);
}
