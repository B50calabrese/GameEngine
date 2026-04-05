/**
 * @file camera_component.h
 * @brief Component that identifies an entity as a camera target or source.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_CAMERA_COMPONENT_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_CAMERA_COMPONENT_H_

namespace engine::ecs::components {

/** @brief Component that allows an entity to act as a camera. */
struct CameraComponent {
  bool is_active = true;
  float zoom = 1.0f;
  // Offset from the entity position
  float offset_x = 0.0f;
  float offset_y = 0.0f;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_CAMERA_COMPONENT_H_
