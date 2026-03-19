#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/scene/scene_manager.h>

#include "scenes.h"

class PlatformerApp : public engine::Application {
 public:
  void OnInit() override {
    engine::SceneManager::Get().SetScene(
        std::make_unique<platformer::MenuScene>("Menu"));
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
