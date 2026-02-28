#include <iostream>
#include <memory>

#include "application.h"
#include "ecs/registry.h"
#include "engine.h"
#include "graphics/renderer.h"
#include "input_manager.h"
#include "scene.h"
#include "scene_manager.h"

constexpr float WIDTH = 800.0f;
constexpr float HEIGHT = 600.0f;

class MainScene : public engine::Scene {
 public:
  MainScene(const std::string& name) : Scene(name) {
    entity_1_ = registry_.create_entity();
    entity_2_ = registry_.create_entity();
  }

  void on_render() override {}

  void on_attach() override {}

  void on_update(float delta_time_seconds) override {
    auto position = engine::InputManager::get().get_mouse_screen_pos();

    std::cout << "X: " << position.x << " Y: " << position.y << "\n";
  }

  bool on_input() override { return true; }

 private:
  engine::ecs::Registry registry_;
  engine::ecs::EntityID entity_1_;
  engine::ecs::EntityID entity_2_;
};

class MyApp : public engine::Application {
 public:
  void on_init() override {
    std::cout << "Initializing Application" << std::endl;
    engine::SceneManager::get().push_scene(
        std::make_unique<MainScene>("SceneA"));
  }

  void on_shutdown() override {
    std::cout << "Shutting down Application" << std::endl;
  }

  void on_update(double delta_time_seconds) override {}

 private:
  double total_time_ = 0.0;
};

/**
 * @brief Demo showing the usage of an entity-component-system framework.
 */
int main(void) {
  engine::EngineConfig engine_config;
  engine_config.window_height = HEIGHT;
  engine_config.window_width = WIDTH;
  engine::Engine::init(engine_config);
  MyApp my_app;
  my_app.run();
  return 0;
}
