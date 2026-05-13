/**
 * @file input_system.h
 * @brief System for handling UI input.
 */

#ifndef INCLUDE_ENGINE_UI_INPUT_SYSTEM_H_
#define INCLUDE_ENGINE_UI_INPUT_SYSTEM_H_

#include <engine/ecs/registry.h>

namespace engine::ui {

/**
 * @brief Handles mouse interactions with UI elements.
 */
class UiInputSystem {
 public:
  static void Update(ecs::Registry& reg);
};

}  // namespace engine::ui

#endif  // INCLUDE_ENGINE_UI_INPUT_SYSTEM_H_
