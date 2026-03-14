#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/core/job_system.h>
#include <engine/graphics/camera.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/util/logger.h>

#include <glm/glm.hpp>
#include <random>
#include <vector>

struct Particle {
  glm::vec2 position;
  glm::vec2 velocity;
  glm::vec4 color;
};

class JobSystemDemo : public engine::Application {
 public:
  JobSystemDemo() {}

  void OnInit() override {
    LOG_INFO("Initializing Job System Demo");
    ResetParticles(50000);
  }

  void OnShutdown() override {
    LOG_INFO("Shutting down Job System Demo");
  }

  void OnUpdate(double delta_time_seconds) override {
    float delta_time = static_cast<float>(delta_time_seconds);

    // Handle input to increase/decrease particles
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_UP)) {
      ResetParticles(particles_.size() + 5000);
    }
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_DOWN)) {
      if (particles_.size() > 5000) {
        ResetParticles(particles_.size() - 5000);
      }
    }

    // Handle input to increase/decrease threads used for calculations
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_RIGHT)) {
      num_threads_++;
      LOG_INFO("Using %d threads for simulation", num_threads_);
    }
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_LEFT)) {
      if (num_threads_ > 1) {
        num_threads_--;
        LOG_INFO("Using %d threads for simulation", num_threads_);
      }
    }

    // Split particle trajectory math across threads
    size_t num_particles = particles_.size();
    if (num_particles > 0) {
      size_t particles_per_thread = num_particles / num_threads_;
      std::vector<std::future<void>> futures;

      for (int i = 0; i < num_threads_; ++i) {
        size_t start = i * particles_per_thread;
        size_t end = (i == num_threads_ - 1) ? num_particles : (i + 1) * particles_per_thread;

        if (start < end) {
          futures.push_back(engine::core::JobSystem::Get().Execute([this, start, end, delta_time]() {
            for (size_t j = start; j < end; ++j) {
              particles_[j].position += particles_[j].velocity * delta_time;

              // Bounce off screen edges
              if (particles_[j].position.x < 0 || particles_[j].position.x > 1280) {
                particles_[j].velocity.x *= -1;
              }
              if (particles_[j].position.y < 0 || particles_[j].position.y > 720) {
                particles_[j].velocity.y *= -1;
              }
            }
          }));
        }
      }

      // Wait for all tasks to complete before drawing
      for (auto& f : futures) {
        f.wait();
      }
    }

    for (const auto& p : particles_) {
      engine::graphics::Renderer::Get().DrawQuad(p.position, {2.0f, 2.0f}, p.color);
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
      particles_.push_back({
          {pos_x(gen), pos_y(gen)},
          {vel(gen), vel(gen)},
          {color(gen), color(gen), color(gen), 1.0f}
      });
    }
    LOG_INFO("Simulating %zu particles", count);
  }

  std::vector<Particle> particles_;
  int num_threads_ = 4;
};

int main() {
  engine::EngineConfig engine_config;
  engine::Engine::Init(engine_config);
  JobSystemDemo demo;
  demo.Run();
  return 0;
}
