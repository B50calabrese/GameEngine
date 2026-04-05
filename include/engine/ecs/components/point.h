/**
 * @file point.h
 * @brief Component for rendering a point.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_POINT_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_POINT_H_

#include <glm/glm.hpp>

namespace engine::ecs::components {

/** @brief Component for rendering a point. */
struct Point {
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  float size = 1.0f;
  float z_index = 0.0f;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_POINT_H_
