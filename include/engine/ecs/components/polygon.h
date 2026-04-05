/**
 * @file polygon.h
 * @brief Component for rendering a convex polygon.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_POLYGON_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_POLYGON_H_

#include <vector>

#include <glm/glm.hpp>

namespace engine::ecs::components {

/** @brief Component for rendering a convex polygon. */
struct Polygon {
  std::vector<glm::vec2> vertices;  // Relative to transform position
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  float z_index = 0.0f;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_POLYGON_H_
