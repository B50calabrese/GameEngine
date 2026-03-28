/**
 * @file script_system.h
 * @brief System for processing Lua scripts.
 */

#ifndef INCLUDE_ENGINE_ECS_SYSTEMS_SCRIPT_SYSTEM_H_
#define INCLUDE_ENGINE_ECS_SYSTEMS_SCRIPT_SYSTEM_H_

#include <engine/ecs/registry.h>

namespace engine::ecs::systems {

/**
 * @brief System that handles Lua script execution and ECS integration.
 */
class ScriptSystem {
 public:
  /**
   * @brief Initializes the ScriptSystem and binds ECS to Lua.
   * @param registry Pointer to the ECS Registry.
   */
  static void Init(Registry* registry);

  /**
   * @brief Updates all entities with ScriptComponents.
   * @param registry Pointer to the ECS Registry.
   * @param dt Delta time in seconds.
   */
  static void Update(Registry* registry, float dt);

 private:
  /** @brief Flag to ensure Init is only called once. */
  static bool initialized_;
};

}  // namespace engine::ecs::systems

#endif  // INCLUDE_ENGINE_ECS_SYSTEMS_SCRIPT_SYSTEM_H_
