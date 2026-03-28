#include <engine/core/engine.h>
#include <engine/graphics/ecs/text_renderer.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>

#include "../../common/menu_scene.h"
#include "scenes.h"

namespace platformer {

LevelCompleteScene::LevelCompleteScene(const std::string& name, int level)
    : engine::Scene(name), level_(level) {}

void LevelCompleteScene::OnAttach() {}

void LevelCompleteScene::OnUpdate(float dt) {
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace) ||
      engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEnter)) {
    if (level_ < 2) {  // We only have 2 levels
      engine::SceneManager::Get().SetScene(
          std::make_unique<GameplayScene>("Gameplay", level_ + 1));
    } else {
      std::vector<demos::common::BaseMenuScene::MenuItem> items = {
          {"Level 1",
           []() {
             engine::SceneManager::Get().SetScene(
                 std::make_unique<platformer::GameplayScene>("Gameplay", 1));
           }},
          {"Level 2",
           []() {
             engine::SceneManager::Get().SetScene(
                 std::make_unique<platformer::GameplayScene>("Gameplay", 2));
           }},
          {"Quit", []() { engine::Engine::Shutdown(); }}};
      engine::SceneManager::Get().SetScene(
          std::make_unique<demos::common::BaseMenuScene>("PLATFORMER DEMO",
                                                         items));
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
