/**
 * @file particle_emitter.h
 * @brief ECS component for a particle emitter.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_PARTICLE_EMITTER_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_PARTICLE_EMITTER_H_

#include <engine/ecs/components/particle_emitter.h>
#include <engine/graphics/utils/particle_system.h>

namespace engine::ecs::components {

/** @brief ECS component to trigger particle emission. */
struct ParticleEmitter {
  graphics::utils::ParticleSystem system;
  float z_index = 0.0f;
  bool is_active = true;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_PARTICLE_EMITTER_H_
