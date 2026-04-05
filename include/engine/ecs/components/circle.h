/**
 * @file circle.h
 * @brief Component for rendering a circle.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_CIRCLE_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_CIRCLE_H_

#include <glm/glm.hpp>

namespace engine::ecs::components {

/** @brief Component for rendering a circle. */
struct Circle {
  float radius = 1.0f;
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  float thickness = 0.0f;  // 0 = Fill, >0 = Outline
  glm::vec4 color2 = {1.0f, 1.0f, 1.0f, 1.0f};
  int gradient_type = 0;  // 0 = None, 1 = Linear, 2 = Radial
  float z_index = 0.0f;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_CIRCLE_H_
