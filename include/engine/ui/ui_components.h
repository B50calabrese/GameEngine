/**
 * @file ui_components.h
 * @brief Components for the Hybrid Retained ECS UI System.
 */

#ifndef INCLUDE_ENGINE_UI_UI_COMPONENTS_H_
#define INCLUDE_ENGINE_UI_UI_COMPONENTS_H_

#include <functional>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <engine/ecs/entity_manager.h>

namespace engine::ui {

/** @brief Base UI Transform. Global values calculated by UILayoutSystem. */
struct UITransform {
  glm::vec2 local_pos = {0.0f, 0.0f};
  glm::vec2 size = {100.0f, 100.0f};
  glm::vec2 global_pos = {0.0f, 0.0f};  // System-calculated

  // Anchoring (0 to 1)
  glm::vec2 anchor_min = {0.0f, 0.0f};
  glm::vec2 anchor_max = {0.0f, 0.0f};

  int z_index = 100;     // UI range: 100 to 200
  bool is_dirty = true;  // Set true when local_pos/parent changes
};

/** @brief Relationship component for nesting. */
struct UIHierarchy {
  ecs::EntityID parent = ecs::INVALID_ENTITY;
  std::vector<ecs::EntityID> children;
};

/** @brief Reactive data binding for text labels. */
struct UIBinding {
  std::function<std::string()> get_text;
  std::string last_value;
};

/** @brief Interaction component for buttons/sliders. */
struct UIInteractable {
  bool is_hovered = false;
  bool is_pressed = false;
  std::function<void()> on_click;
  std::function<void(bool)> on_hover_changed;
  std::function<void(float)> on_value_changed;  // For sliders
};

}  // namespace engine::ui

#endif  // INCLUDE_ENGINE_UI_UI_COMPONENTS_H_
