/**
 * @file line.h
 * @brief Component for rendering a line.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_LINE_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_LINE_H_

#include <glm/glm.hpp>

namespace engine::ecs::components {

/** @brief Component for rendering a line to a destination. */
struct Line {
  glm::vec2 dest = {0.0f, 0.0f};  // Relative or absolute destination? Usually relative for components.
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  float thickness = 1.0f;
  bool is_dashed = false;
  float z_index = 0.0f;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_LINE_H_
