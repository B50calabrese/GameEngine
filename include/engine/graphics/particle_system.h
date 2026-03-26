/**
 * @file particle_system.h
 * @brief Generic particle system for effects.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_PARTICLE_SYSTEM_H_
#define INCLUDE_ENGINE_GRAPHICS_PARTICLE_SYSTEM_H_

#include <vector>

#include <glm/glm.hpp>

namespace engine::graphics {

/** @brief Representation of a single particle. */
struct Particle {
  glm::vec2 position = {0.0f, 0.0f};
  glm::vec2 velocity = {0.0f, 0.0f};
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  float size = 5.0f;
  float lifetime = 1.0f;
  float max_lifetime = 1.0f;
};

/** @brief Manages a collection of particles. */
class ParticleSystem {
 public:
  ParticleSystem(size_t max_particles = 1000);

  /**
   * @brief Emits a burst of particles.
   * @param position Source position.
   * @param count Number of particles.
   * @param base_velocity Base velocity.
   * @param velocity_range Random variance for velocity.
   * @param color Base color.
   * @param lifetime_base Base lifetime.
   */
  void Emit(const glm::vec2& position, int count,
            const glm::vec2& base_velocity = {0, 0},
            const glm::vec2& velocity_range = {50, 50},
            const glm::vec4& color = {1, 1, 1, 1}, float lifetime_base = 1.0f);

  /**
   * @brief Updates particle positions and lifetimes.
   * @param dt Delta time.
   */
  void Update(float dt);

  /**
   * @brief Renders the particles.
   * @param z_index The Z-order for sorting.
   */
  void Render(float z_index = 0.0f) const;

  /** @brief Clears all active particles. */
  void Clear();

 private:
  std::vector<Particle> particles_;
  size_t max_particles_;
};

/** @brief ECS component to trigger particle emission. */
struct ParticleEmitterComponent {
  ParticleSystem system;
  float z_index = 0.0f;
  bool is_active = true;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_PARTICLE_SYSTEM_H_
