/**
 * @file ui_binding.h
 * @brief Reactive data binding for UI elements.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_UI_BINDING_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_UI_BINDING_H_

#include <functional>
#include <string>

namespace engine::ecs::components {

/**
 * @brief Reactive data binding for text labels.
 */
struct UiBinding {
  std::function<std::string()> get_text;
  std::string last_value;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_UI_BINDING_H_
