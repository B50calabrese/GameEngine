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
  SceneBOverlay(const std::string& name) : Scene(name) {}

  void OnRender() override {
    // Draw a green rectangle on some of the screen.
    engine::graphics::Renderer::Get().DrawRect(WIDTH / 4.0f, HEIGHT / 4.0f,
                                               WIDTH / 2.0f, HEIGHT / 2.0f,
                                               0.0f, 1.0f, 0.0f);
  }

  bool OnInput() override {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_ESCAPE)) {
      // Remove the current screen.
      engine::SceneManager::Get().PopScene();
    }
    return true;
  }
};

class SceneB : public engine::Scene {
 public:
  SceneB(const std::string& name) : Scene(name) {}

  void OnRender() override {
    // Draw a blue rectangle on the whole screen.
    engine::graphics::Renderer::Get().DrawRect(0.0f, 0.0f, WIDTH, HEIGHT, 0.0f,
                                               0.0f, 1.0f);
  }

  bool OnInput() override {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_ESCAPE)) {
      // Push the overlay screen.
      engine::SceneManager::Get().PushScene(
          std::make_unique<SceneBOverlay>("SceneBOverlay"));
    }
    return true;
  }
};

class SceneA : public engine::Scene {
 public:
  SceneA(const std::string& name) : Scene(name) {}

  void OnRender() override {
    // Draw a red rectangle on the whole screen.
    engine::graphics::Renderer::Get().DrawRect(0.0f, 0.0f, WIDTH, HEIGHT, 1.0f,
                                               0.0f, 0.0f);
  }

  bool OnInput() override {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_SPACE)) {
      engine::SceneManager::Get().SetScene(std::make_unique<SceneB>("SceneB"));
    }
    return true;
  }
};

class MyApp : public engine::Application {
 public:
  void OnInit() override {
    std::cout << "Initializing Application" << std::endl;
    engine::SceneManager::Get().PushScene(std::make_unique<SceneA>("SceneA"));
  }

  void OnShutdown() override {
    std::cout << "Shutting down Application" << std::endl;
  }

  void OnUpdate(double deltaTimeSeconds) override {}

 private:
  double totalTime_ = 0.0;
};

/**
 * Demo showing the full end-to-end management of an application.
 */
int main(void) {
  engine::EngineConfig engine_config;
  engine_config.window_height = HEIGHT;
  engine_config.window_width = WIDTH;
  engine::Engine::Init(engine_config);
  MyApp myApp;
  myApp.Run();
  return 0;
}