#include <random>
#include <vector>

#include <glm/glm.hpp>

#include <engine/core/job_system.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/util/logger.h>

#include "../common/demo_utils.h"

struct Particle {
  glm::vec2 position;
  glm::vec2 velocity;
  glm::vec4 color;
};

class JobSystemDemo : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    LOG_INFO("Initializing Job System Demo");
    ResetParticles(50000);
  }

  void OnDemoShutdown() override { LOG_INFO("Shutting down Job System Demo"); }

  void OnDemoUpdate(double delta_time_seconds) override {
    float delta_time = static_cast<float>(delta_time_seconds);

    // Handle input to increase/decrease particles
    if (input_manager().IsKeyPressed(engine::KeyCode::kUp)) {
      ResetParticles(particles_.size() + 5000);
    }
    if (input_manager().IsKeyPressed(engine::KeyCode::kDown)) {
      if (particles_.size() > 5000) {
        ResetParticles(particles_.size() - 5000);
      }
    }

    // Handle input to increase/decrease threads used for calculations
    if (input_manager().IsKeyPressed(engine::KeyCode::kRight)) {
      num_threads_++;
      LOG_INFO("Using {} threads for simulation", num_threads_);
    }
    if (input_manager().IsKeyPressed(engine::KeyCode::kLeft)) {
      if (num_threads_ > 1) {
        num_threads_--;
        LOG_INFO("Using {} threads for simulation", num_threads_);
      }
    }

    // Split particle trajectory math across threads
    size_t num_particles = particles_.size();
    if (num_particles > 0) {
      size_t particles_per_thread = num_particles / num_threads_;

      for (int i = 0; i < num_threads_; ++i) {
        size_t start = i * particles_per_thread;
        size_t end = (i == num_threads_ - 1) ? num_particles
                                             : (i + 1) * particles_per_thread;

        if (start < end) {
          engine::core::JobSystem::Get().Execute(
              [this, start, end, delta_time]() {
                for (size_t j = start; j < end; ++j) {
                  particles_[j].position += particles_[j].velocity * delta_time;

                  // Bounce off screen edges
                  if (particles_[j].position.x < 0 ||
                      particles_[j].position.x > 1280) {
                    particles_[j].velocity.x *= -1;
                  }
                  if (particles_[j].position.y < 0 ||
                      particles_[j].position.y > 720) {
                    particles_[j].velocity.y *= -1;
                  }
                }
              });
        }
      }
      // Explicitly wait for calculations to finish before we start reading
      // positions for drawing.
      engine::core::JobSystem::Get().Wait();
    }

    for (const auto& p : particles_) {
      engine::graphics::Renderer::Get().DrawQuad(p.position, {2.0f, 2.0f},
                                                 p.color);
    }
  }

 private:
  void ResetParticles(size_t count) {
    particles_.clear();
    particles_.reserve(count);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> pos_x(0, 1280);
    std::uniform_real_distribution<float> pos_y(0, 720);
    std::uniform_real_distribution<float> vel(-100, 100);
    std::uniform_real_distribution<float> color(0, 1);

    for (size_t i = 0; i < count; ++i) {
      particles_.push_back({{pos_x(gen), pos_y(gen)},
                            {vel(gen), vel(gen)},
                            {color(gen), color(gen), color(gen), 1.0f}});
    }
    LOG_INFO("Simulating {} particles", count);
  }

  std::vector<Particle> particles_;
  int num_threads_ = 4;
};

int main() {
  engine::EngineConfig engine_config;
  return demos::common::DemoRunner::Run<JobSystemDemo>(engine_config);
}
