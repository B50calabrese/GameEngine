#include <memory>

#include <engine/ecs/registry.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>

#include "../common/demo_utils.h"

constexpr float WIDTH = 800.0f;
constexpr float HEIGHT = 600.0f;

class MainScene : public engine::Scene {
 public:
  MainScene(const std::string& name) : Scene(name) {
    entity_1_ = registry_.CreateEntity();
    entity_2_ = registry_.CreateEntity();
  }

  void OnUpdate(float delta_time_seconds) override {
    auto position = engine::InputManager::Get().mouse_screen_pos();
    LOG_INFO("Mouse X: {}, Y: {}", position.x, position.y);
  }

 private:
  engine::ecs::Registry registry_;
  engine::ecs::EntityID entity_1_;
  engine::ecs::EntityID entity_2_;
};

class EcsApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    LOG_INFO("Initializing ECS Demo");
    engine::SceneManager::Get().PushScene(
        std::make_unique<MainScene>("SceneA"));
  }

  void OnDemoShutdown() override {
    LOG_INFO("Shutting down ECS Demo");
  }
};

/**
 * @brief Demo showing the usage of an entity-component-system framework.
 */
int main(void) {
  engine::EngineConfig engine_config;
  engine_config.window_height = HEIGHT;
  engine_config.window_width = WIDTH;
  return demos::common::DemoRunner::Run<EcsApp>(engine_config);
}
