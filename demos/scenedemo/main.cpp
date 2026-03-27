#include <memory>

#include <engine/graphics/renderer.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>

#include "../common/demo_utils.h"

constexpr float WIDTH = 800.0f;
constexpr float HEIGHT = 600.0f;

class SceneBOverlay : public engine::Scene {
 public:
  explicit SceneBOverlay(const std::string& name) : engine::Scene(name) {}

  void OnRender() override {
    // Draw a green rectangle on some of the screen.
    engine::graphics::Renderer::Get().DrawRect(WIDTH / 4.0f, HEIGHT / 4.0f,
                                               WIDTH / 2.0f, HEIGHT / 2.0f,
                                               0.0f, 1.0f, 0.0f);
  }

  bool OnInput() override {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEscape)) {
      // Remove the current screen.
      engine::SceneManager::Get().PopScene();
    }
    return true;
  }
};

class SceneB : public engine::Scene {
 public:
  explicit SceneB(const std::string& name) : engine::Scene(name) {}

  void OnRender() override {
    // Draw a blue rectangle on the whole screen.
    engine::graphics::Renderer::Get().DrawRect(0.0f, 0.0f, WIDTH, HEIGHT, 0.0f,
                                               0.0f, 1.0f);
  }

  bool OnInput() override {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEscape)) {
      // Push the overlay screen.
      engine::SceneManager::Get().PushScene(
          std::make_unique<SceneBOverlay>("SceneBOverlay"));
    }
    return true;
  }
};

class SceneA : public engine::Scene {
 public:
  explicit SceneA(const std::string& name) : engine::Scene(name) {}

  void OnRender() override {
    // Draw a red rectangle on the whole screen.
    engine::graphics::Renderer::Get().DrawRect(0.0f, 0.0f, WIDTH, HEIGHT, 1.0f,
                                               0.0f, 0.0f);
  }

  bool OnInput() override {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace)) {
      engine::SceneManager::Get().SetScene(std::make_unique<SceneB>("SceneB"));
    }
    return true;
  }
};

class SceneApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    LOG_INFO("Initializing Scene Demo");
    engine::SceneManager::Get().PushScene(std::make_unique<SceneA>("SceneA"));
  }

  void OnDemoShutdown() override {
    LOG_INFO("Shutting down Scene Demo");
  }
};

/**
 * @brief Demo showing the full end-to-end management of an application.
 */
int main(void) {
  engine::EngineConfig engine_config;
  engine_config.window_height = HEIGHT;
  engine_config.window_width = WIDTH;
  return demos::common::DemoRunner::Run<SceneApp>(engine_config);
}
