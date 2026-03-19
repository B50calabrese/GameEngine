#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>

#include "scenes.h"

namespace platformer {

LevelCompleteScene::LevelCompleteScene(const std::string& name, int level)
    : engine::Scene(name), level_(level) {}

void LevelCompleteScene::OnAttach() {
  engine::graphics::TextRenderer::Get().Init();
  engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
}

void LevelCompleteScene::OnUpdate(float dt) {
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_SPACE) ||
      engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_ENTER)) {
    if (level_ < 3) {
      engine::SceneManager::Get().SetScene(
          std::make_unique<GameplayScene>("Gameplay", level_ + 1));
    } else {
      engine::SceneManager::Get().SetScene(std::make_unique<MenuScene>("Menu"));
    }
  }
}

void LevelCompleteScene::OnRender() {
  engine::graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                             {0.0f, 0.0f, 0.0f, 0.8f});
  std::string msg = "LEVEL " + std::to_string(level_) + " COMPLETE!";
  engine::graphics::Renderer::Get().DrawText(
      "default", msg, {200.0f, 400.0f}, 0.0f, 2.0f, {1.0f, 1.0f, 0.0f, 1.0f});
  engine::graphics::Renderer::Get().DrawText(
      "default", "Press SPACE to Continue", {250.0f, 300.0f}, 0.0f, 1.0f,
      {1.0f, 1.0f, 1.0f, 1.0f});
  engine::graphics::Renderer::Get().Flush();
}

}  // namespace platformer
