/**
 * @file particle_system.cpp
 * @brief Implementation of the particle system.
 */

#include <algorithm>
#include <random>

#include <engine/graphics/particle_system.h>
#include <engine/graphics/renderer.h>

namespace engine::graphics {

ParticleSystem::ParticleSystem(size_t max_particles)
    : max_particles_(max_particles) {
  particles_.reserve(max_particles);
}

void ParticleSystem::Emit(const glm::vec2& position, int count,
                          const glm::vec2& base_velocity,
                          const glm::vec2& velocity_range,
                          const glm::vec4& color, float lifetime_base) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis_vx(-velocity_range.x,
                                               velocity_range.x);
  std::uniform_real_distribution<float> dis_vy(-velocity_range.y,
                                               velocity_range.y);
  std::uniform_real_distribution<float> dis_life(0.0f, 0.5f);

  for (int i = 0; i < count; ++i) {
    if (particles_.size() >= max_particles_) {
      break;
    }

    Particle p;
    p.position = position;
    p.velocity = base_velocity + glm::vec2(dis_vx(gen), dis_vy(gen));
    p.color = color;
    p.lifetime = lifetime_base + dis_life(gen);
    p.max_lifetime = p.lifetime;
    particles_.push_back(p);
  }
}

void ParticleSystem::Update(float dt) {
  for (auto it = particles_.begin(); it != particles_.end();) {
    it->position += it->velocity * dt;
    it->lifetime -= dt;

    // Fade out
    it->color.a = std::max(0.0f, it->lifetime / it->max_lifetime);

    if (it->lifetime <= 0) {
      it = particles_.erase(it);
    } else {
      ++it;
    }
  }
}

void ParticleSystem::Render() const {
  for (const auto& p : particles_) {
    Renderer::Get().DrawQuad(p.position, {p.size, p.size}, p.color);
  }
}

void ParticleSystem::Clear() { particles_.clear(); }

}  // namespace engine::graphics
