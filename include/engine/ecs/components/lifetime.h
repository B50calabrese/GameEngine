/**
 * @file lifetime.h
 * @brief Component for automatic entity destruction.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_LIFETIME_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_LIFETIME_H_

namespace engine::ecs::components {

/** @brief Component that defines how long an entity exists. */
struct Lifetime {
  float remaining = 1.0f;  // Seconds until destruction
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_LIFETIME_H_
