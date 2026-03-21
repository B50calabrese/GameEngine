/**
 * @file menu_scene.cpp
 * @brief Base menu scene implementation.
 */

#include "menu_scene.h"

#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/input_manager.h>

namespace demos::common {

BaseMenuScene::BaseMenuScene(const std::string& title,
                             const std::vector<MenuItem>& items)
    : engine::Scene("Menu"), title_(title), items_(items) {}

void BaseMenuScene::OnAttach() {
  engine::graphics::TextRenderer::Get().Init();
  engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
}

void BaseMenuScene::OnUpdate(float dt) {
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kW) ||
      engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kUp)) {
    selected_index_ =
        (selected_index_ - 1 + static_cast<int>(items_.size())) % items_.size();
  }
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kS) ||
      engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kDown)) {
    selected_index_ = (selected_index_ + 1) % items_.size();
  }
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace) ||
      engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kEnter)) {
    if (selected_index_ >= 0 && selected_index_ < items_.size()) {
      items_[selected_index_].action();
    }
  }
}

void BaseMenuScene::OnRender() {
  // Simple dark background
  engine::graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                             {0.05f, 0.05f, 0.1f, 1.0f});

  // Title
  engine::graphics::Renderer::Get().DrawText(
      "default", title_, {200.0f, 450.0f}, 0.0f, 1.5f, title_color_);

  // Items
  for (int i = 0; i < items_.size(); ++i) {
    glm::vec4 color = (i == selected_index_) ? selected_color_ : item_color_;
    float y = 350.0f - (i * 40.0f);
    engine::graphics::Renderer::Get().DrawText("default", items_[i].text,
                                               {250.0f, y}, 0.0f, 1.0f, color);
  }

  engine::graphics::Renderer::Get().Flush();
}

}  // namespace demos::common
