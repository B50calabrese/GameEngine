/**
 * @file ui_transform.h
 * @brief Base UI Transform component.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_UI_TRANSFORM_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_UI_TRANSFORM_H_

#include <glm/glm.hpp>

namespace engine::ecs::components {

/**
 * @brief Base UI Transform. Global values calculated by UiLayoutSystem.
 */
struct UiTransform {
  glm::vec2 local_pos = {0.0f, 0.0f};
  glm::vec2 size = {100.0f, 100.0f};
  glm::vec2 global_pos = {0.0f, 0.0f};  // System-calculated

  // Anchoring (0 to 1)
  glm::vec2 anchor_min = {0.0f, 0.0f};
  glm::vec2 anchor_max = {0.0f, 0.0f};

  int z_index = 100;     // UI range: 100 to 200
  bool is_dirty = true;  // Set true when local_pos/parent changes
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_UI_TRANSFORM_H_
