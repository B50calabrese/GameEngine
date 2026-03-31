#include <engine/core/engine.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>

#include "scenes.h"

namespace platformer {

MenuScene::MenuScene(const std::string& name) : engine::Scene(name) {}

void MenuScene::OnAttach() {}

void MenuScene::OnUpdate(float dt) {
  if (show_controls_) {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEscape) ||
        engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace) ||
        engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEnter)) {
      show_controls_ = false;
    }
    return;
  }

  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::k1) ||
      engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace) ||
      engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEnter)) {
    engine::SceneManager::Get().SetScene(
        std::make_unique<GameplayScene>("Gameplay", 1));
  } else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::k2)) {
    show_controls_ = true;
  } else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::k3) ||
             engine::InputManager::Get().IsKeyPressed(
                 engine::KeyCode::kEscape)) {
    engine::Engine::Shutdown();
  }
}

void MenuScene::OnRender() {
  engine::graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                             {0.1f, 0.1f, 0.15f, 1.0f});

  if (show_controls_) {
    engine::graphics::Renderer::Get().DrawQuad("default", "CONTROLS",
                                               {300.0f, 450.0f}, 0.0f, 1.5f,
                                               {1.0f, 1.0f, 0.0f, 1.0f});
    engine::graphics::Renderer::Get().DrawQuad("default", "ARROWS: Move",
                                               {300.0f, 350.0f}, 0.0f, 1.0f,
                                               {1.0f, 1.0f, 1.0f, 1.0f});
    engine::graphics::Renderer::Get().DrawQuad("default", "SPACE: Jump",
                                               {300.0f, 300.0f}, 0.0f, 1.0f,
                                               {1.0f, 1.0f, 1.0f, 1.0f});
    engine::graphics::Renderer::Get().DrawQuad(
        "default", "Press SPACE to go back", {250.0f, 150.0f}, 0.0f, 0.8f,
        {0.7f, 0.7f, 0.7f, 1.0f});
  } else {
    engine::graphics::Renderer::Get().DrawQuad("default", "PLATFORMER DEMO",
                                               {200.0f, 450.0f}, 0.0f, 2.0f,
                                               {0.0f, 1.0f, 0.5f, 1.0f});
    engine::graphics::Renderer::Get().DrawQuad("default", "1. Start Game",
                                               {300.0f, 300.0f}, 0.0f, 1.2f,
                                               {1.0f, 1.0f, 1.0f, 1.0f});
    engine::graphics::Renderer::Get().DrawQuad("default", "2. Controls",
                                               {300.0f, 250.0f}, 0.0f, 1.2f,
                                               {1.0f, 1.0f, 1.0f, 1.0f});
    engine::graphics::Renderer::Get().DrawQuad("default", "3. Exit",
                                               {300.0f, 200.0f}, 0.0f, 1.2f,
                                               {1.0f, 1.0f, 1.0f, 1.0f});
  }
  engine::graphics::Renderer::Get().Flush();
}

}  // namespace platformer
