/**
 * @file ai_system.h
 * @brief System that updates AI components (FSM and Behavior Tree).
 */

#ifndef INCLUDE_ENGINE_ECS_AI_SYSTEM_H_
#define INCLUDE_ENGINE_ECS_AI_SYSTEM_H_

#include <engine/ecs/registry.h>

namespace engine::ecs {

/**
 * @brief System that updates all FSM and Behavior Tree components in the
 * registry.
 */
class AISystem {
 public:
  /**
   * @brief Updates all AI components.
   * @param registry The ECS registry.
   * @param dt Delta time since the last frame.
   */
  static void Update(Registry* registry, float dt);
};

}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_AI_SYSTEM_H_
