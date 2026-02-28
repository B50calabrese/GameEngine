#include <iostream>
#include <memory>

#include "application.h"
#include "engine.h"
#include "graphics/renderer.h"
#include "input_manager.h"
#include "scene.h"
#include "scene_manager.h"

constexpr float WIDTH = 800.0f;
constexpr float HEIGHT = 600.0f;

class SceneBOverlay : public engine::Scene {
 public:
  explicit SceneBOverlay(const std::string& name) : Scene(name) {}

  void on_render() override {
    // Draw a green rectangle on some of the screen.
    engine::graphics::Renderer::get().draw_rect(WIDTH / 4.0f, HEIGHT / 4.0f,
                                                WIDTH / 2.0f, HEIGHT / 2.0f,
                                                0.0f, 1.0f, 0.0f);
  }

  bool on_input() override {
    if (engine::InputManager::get().is_key_pressed(engine::KeyCode::KC_ESCAPE)) {
      // Remove the current screen.
      engine::SceneManager::get().pop_scene();
    }
    return true;
  }
};

class SceneB : public engine::Scene {
 public:
  explicit SceneB(const std::string& name) : Scene(name) {}

  void on_render() override {
    // Draw a blue rectangle on the whole screen.
    engine::graphics::Renderer::get().draw_rect(0.0f, 0.0f, WIDTH, HEIGHT, 0.0f,
                                                0.0f, 1.0f);
  }

  bool on_input() override {
    if (engine::InputManager::get().is_key_pressed(engine::KeyCode::KC_ESCAPE)) {
      // Push the overlay screen.
      engine::SceneManager::get().push_scene(
          std::make_unique<SceneBOverlay>("SceneBOverlay"));
    }
    return true;
  }
};

class SceneA : public engine::Scene {
 public:
  explicit SceneA(const std::string& name) : Scene(name) {}

  void on_render() override {
    // Draw a red rectangle on the whole screen.
    engine::graphics::Renderer::get().draw_rect(0.0f, 0.0f, WIDTH, HEIGHT, 1.0f,
                                                0.0f, 0.0f);
  }

  bool on_input() override {
    if (engine::InputManager::get().is_key_pressed(engine::KeyCode::KC_SPACE)) {
      engine::SceneManager::get().set_scene(std::make_unique<SceneB>("SceneB"));
    }
    return true;
  }
};

class MyApp : public engine::Application {
 public:
  void on_init() override {
    std::cout << "Initializing Application" << std::endl;
    engine::SceneManager::get().push_scene(std::make_unique<SceneA>("SceneA"));
  }

  void on_shutdown() override {
    std::cout << "Shutting down Application" << std::endl;
  }

  void on_update(double delta_time_seconds) override {}

 private:
  double total_time_ = 0.0;
};

/**
 * @brief Demo showing the full end-to-end management of an application.
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
