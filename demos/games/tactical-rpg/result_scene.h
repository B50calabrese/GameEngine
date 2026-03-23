#ifndef DEMOS_GAMES_TACTICAL_RPG_RESULT_SCENE_H_
#define DEMOS_GAMES_TACTICAL_RPG_RESULT_SCENE_H_

#include <string>

#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>

namespace tactical_rpg {

class ResultScene : public engine::Scene {
 public:
  ResultScene(const std::string& name, const std::string& message,
              bool is_victory)
      : engine::Scene(name), message_(message), is_victory_(is_victory) {}

  void OnAttach() override {
    engine::graphics::TextRenderer::Get().Init();
    engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
  }

  void OnUpdate(float dt) override {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace)) {
      // Return to main menu
      // For simplicity, we just pop or set scene
      // We'll need to include MainMenuScene.h or use a callback
      ReturnToMenu();
    }
  }

  void ReturnToMenu();

  void OnRender() override {
    engine::graphics::Renderer::Get().DrawQuad({0, 0}, {1280, 720},
                                               {0, 0, 0, 0.8f});
    engine::graphics::Renderer::Get().DrawText(
        "default", is_victory_ ? "VICTORY" : "GAME OVER", {500, 500}, 0.0f,
        2.0f, is_victory_ ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1));
    engine::graphics::Renderer::Get().DrawText("default", message_, {400, 400},
                                               0.0f, 1.0f, {1, 1, 1, 1});
    engine::graphics::Renderer::Get().DrawText(
        "default", "Press SPACE to return to Main Menu", {400, 200}, 0.0f, 0.8f,
        {1, 1, 1, 1});
    engine::graphics::Renderer::Get().Flush();
  }

 private:
  std::string message_;
  bool is_victory_;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_RESULT_SCENE_H_
