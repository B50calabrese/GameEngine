/**
 * @file waypoint_path.h
 * @brief Component for basic waypoint-based pathing.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_WAYPOINT_PATH_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_WAYPOINT_PATH_H_

#include <vector>

#include <glm/glm.hpp>

namespace engine::ecs::components {

/** @brief Component that defines a path for an entity to follow. */
struct WaypointPath {
  std::vector<glm::vec2> points;  // Path points in world space
  int current_index = 0;          // Next waypoint to move towards
  float speed = 100.0f;           // Speed in units/sec
  bool loop = true;               // Loop back to first point
  bool finished = false;          // Path following finished
  float arrival_threshold = 5.0f; // Distance to consider waypoint reached
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_WAYPOINT_PATH_H_
