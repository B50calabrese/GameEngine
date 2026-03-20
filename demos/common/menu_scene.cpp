/**
 * @file menu_scene.cpp
 * @brief Base menu scene implementation.
 */

#include "menu_scene.h"
#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/input_manager.h>

namespace demos::common {

using namespace engine;

BaseMenuScene::BaseMenuScene(const std::string& title, const std::vector<MenuItem>& items)
    : Scene("Menu"), title_(title), items_(items) {}

void BaseMenuScene::OnAttach() {
  graphics::TextRenderer::Get().Init();
  graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
}

void BaseMenuScene::OnUpdate(float dt) {
  if (InputManager::Get().IsKeyPressed(KeyCode::KC_W) || InputManager::Get().IsKeyPressed(KeyCode::KC_UP)) {
    selected_index_ = (selected_index_ - 1 + static_cast<int>(items_.size())) % items_.size();
  }
  if (InputManager::Get().IsKeyPressed(KeyCode::KC_S) || InputManager::Get().IsKeyPressed(KeyCode::KC_DOWN)) {
    selected_index_ = (selected_index_ + 1) % items_.size();
  }
  if (InputManager::Get().IsKeyPressed(KeyCode::KC_SPACE) || InputManager::Get().IsKeyPressed(KeyCode::KC_ENTER)) {
    if (selected_index_ >= 0 && selected_index_ < items_.size()) {
      items_[selected_index_].action();
    }
  }
}

void BaseMenuScene::OnRender() {
  // Simple dark background
  graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f}, {0.05f, 0.05f, 0.1f, 1.0f});

  // Title
  graphics::Renderer::Get().DrawText("default", title_, {200.0f, 450.0f}, 0.0f, 1.5f, title_color_);

  // Items
  for (int i = 0; i < items_.size(); ++i) {
    glm::vec4 color = (i == selected_index_) ? selected_color_ : item_color_;
    float y = 350.0f - (i * 40.0f);
    graphics::Renderer::Get().DrawText("default", items_[i].text, {250.0f, y}, 0.0f, 1.0f, color);
  }

  graphics::Renderer::Get().Flush();
}

}  // namespace demos::common
