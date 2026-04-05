/**
 * @file triangle.h
 * @brief Component for rendering a triangle.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_TRIANGLE_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_TRIANGLE_H_

#include <glm/glm.hpp>

namespace engine::ecs::components {

/** @brief Component for rendering a triangle. */
struct Triangle {
  glm::vec2 size = {1.0f, 1.0f};
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec2 origin = {0.5f, 0.5f};
  float thickness = 0.0f;
  glm::vec4 color2 = {1.0f, 1.0f, 1.0f, 1.0f};
  int gradient_type = 0;
  float z_index = 0.0f;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_TRIANGLE_H_
