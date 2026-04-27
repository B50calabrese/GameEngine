#ifndef DEMOS_GAMES_TACTICAL_RPG_MAIN_MENU_SCENE_H_
#define DEMOS_GAMES_TACTICAL_RPG_MAIN_MENU_SCENE_H_

#include <string>
#include <vector>

#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>

#include "game_types.h"

namespace tactical_rpg {

class MainMenuScene : public engine::Scene {
 public:
  MainMenuScene(const std::string& name) : engine::Scene(name) {}

  void OnUpdate(float dt) override {
    auto& input = engine::InputManager::Get();
    if (input.IsKeyPressed(engine::KeyCode::kEnter)) {
      // Switch to character selection (for simplicity, we'll implement it here)
      is_selecting_ = true;
    }

    if (is_selecting_) {
      HandleSelection();
    }
  }

  void HandleSelection() {
    auto& input = engine::InputManager::Get();
    if (input.IsKeyPressed(engine::KeyCode::kW))
      selected_class_index_ = (selected_class_index_ - 1 + 12) % 12;
    if (input.IsKeyPressed(engine::KeyCode::kS))
      selected_class_index_ = (selected_class_index_ + 1) % 12;

    if (input.IsKeyPressed(engine::KeyCode::kSpace)) {
      selected_classes_.push_back(
          static_cast<ClassType>(selected_class_index_));
      if (selected_classes_.size() >= 4) {
        // Start game logic
        StartGame();
      }
    }
  }

  void StartGame();

  void OnRender() override {
    if (!is_selecting_) {
      engine::graphics::TextRenderer::Get().DrawText(
          "default", "TACTICAL RPG ROGUELIKE", {200, 500}, 0.0f, 1.5f,
          {1, 1, 1, 1});
      engine::graphics::TextRenderer::Get().DrawText(
          "default", "Press ENTER to Start Party Selection", {300, 300}, 0.0f,
          0.8f, {1, 1, 1, 1});
    } else {
      engine::graphics::TextRenderer::Get().DrawText(
          "default", "SELECT YOUR PARTY (Choose 4)", {100, 600}, 0.0f, 1.0f,
          {1, 1, 1, 1});
      for (int i = 0; i < 12; ++i) {
        glm::vec4 color = (i == selected_class_index_) ? glm::vec4(1, 1, 0, 1)
                                                       : glm::vec4(1, 1, 1, 1);
        engine::graphics::TextRenderer::Get().DrawText(
            "default", ClassTypeToString(static_cast<ClassType>(i)),
            {150, 550 - i * 40.0f}, 0.0f, 0.7f, color);
      }

      engine::graphics::TextRenderer::Get().DrawText(
          "default",
          "SELECTED: " + std::to_string(selected_classes_.size()) + "/4",
          {500, 550}, 0.0f, 0.8f, {1, 1, 1, 1});
      for (int i = 0; i < (int)selected_classes_.size(); ++i) {
        engine::graphics::TextRenderer::Get().DrawText(
            "default", ClassTypeToString(selected_classes_[i]),
            {500, 500 - i * 40.0f}, 0.0f, 0.7f, {0, 1, 0, 1});
      }
    }
    engine::graphics::Renderer::Get().Flush();
  }

 private:
  bool is_selecting_ = false;
  int selected_class_index_ = 0;
  std::vector<ClassType> selected_classes_;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_MAIN_MENU_SCENE_H_
