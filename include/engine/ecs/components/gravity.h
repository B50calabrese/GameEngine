/**
 * @file gravity.h
 * @brief Component that applies a gravity force to the entity.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_GRAVITY_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_GRAVITY_H_

namespace engine::ecs::components {

/** @brief Component that applies a gravity force to the entity. */
struct Gravity {
  float strength = 1500.0f;  // Default gravity constant
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_GRAVITY_H_
