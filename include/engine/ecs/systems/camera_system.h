/**
 * @file camera_system.h
 * @brief System that syncs the engine camera with a CameraComponent.
 */

#ifndef INCLUDE_ENGINE_ECS_SYSTEMS_CAMERA_SYSTEM_H_
#define INCLUDE_ENGINE_ECS_SYSTEMS_CAMERA_SYSTEM_H_

#include <engine/ecs/registry.h>

namespace engine::ecs::systems {

/**
 * @brief System that updates the main camera based on entities with CameraComponent.
 */
class CameraSystem {
 public:
  /**
   * @brief Updates the main camera.
   * @param registry The ECS registry.
   */
  static void Update(Registry* registry);
};

}  // namespace engine::ecs::systems

#endif  // INCLUDE_ENGINE_ECS_SYSTEMS_CAMERA_SYSTEM_H_
