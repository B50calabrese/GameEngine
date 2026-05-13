/**
 * @file ui_interactable.h
 * @brief Interaction component for UI elements.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_UI_INTERACTABLE_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_UI_INTERACTABLE_H_

#include <functional>

namespace engine::ecs::components {

/**
 * @brief Interaction component for buttons/sliders.
 */
struct UiInteractable {
  bool is_hovered = false;
  bool is_pressed = false;
  std::function<void()> on_click;
  std::function<void(bool)> on_hover_changed;
  std::function<void(float)> on_value_changed;  // For sliders
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_UI_INTERACTABLE_H_
