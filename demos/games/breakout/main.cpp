#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/graphics_components.h>
#include <engine/graphics/post_processor.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/ui/ui_components.h>
#include <engine/ui/ui_systems.h>
#include <engine/util/easing.h>
#include <engine/util/tween_manager.h>

using namespace engine;

// --- Components ---

struct PaddleComponent {
  float speed = 500.0f;
  glm::vec2 base_size = {120.0f, 20.0f};
  glm::vec2 current_size = {120.0f, 20.0f};
  float spring_velocity = 0.0f;
};

struct BallComponent {
  glm::vec2 velocity = {300.0f, -300.0f};
  float radius = 10.0f;
};

struct BrickComponent {
  bool is_destroyed = false;
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
};

struct Particle {
  glm::vec2 position;
  glm::vec2 velocity;
  glm::vec4 color;
  float lifetime;
};

struct ParticleEmitterComponent {
  std::vector<Particle> particles;
};

// --- Helper Functions ---

bool CheckCollision(const glm::vec2& pos1, const glm::vec2& size1,
                    const glm::vec2& pos2, const glm::vec2& size2) {
  bool collision_x = pos1.x + size1.x >= pos2.x && pos2.x + size2.x >= pos1.x;
  bool collision_y = pos1.y + size1.y >= pos2.y && pos2.y + size2.y >= pos1.y;
  return collision_x && collision_y;
}

// --- Scenes ---

class GameplayScene : public Scene {
 public:
  GameplayScene(const std::string& name) : Scene(name) {}

  void OnAttach() override {
    graphics::TextRenderer::Get().Init();
#ifdef COMMON_ASSETS_PATH
    graphics::TextRenderer::Get().LoadFont("default",
                                           COMMON_ASSETS_PATH "arial.ttf", 24);
#else
    graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
#endif

    ResetGame();
  }

  void ResetGame() {
    registry().Clear();
    bricks_hit_ = 0;
    bricks_.clear();

    // Create Paddle
    paddle_ = registry().CreateEntity();
    paddle_pos_ = {400.0f, 550.0f};
    registry().AddComponent(paddle_, PaddleComponent{});

    // Create Ball
    ball_ = registry().CreateEntity();
    ball_pos_ = {400.0f, 500.0f};
    registry().AddComponent(ball_, BallComponent{});

    // Create Bricks
    float brick_width = 75.0f;
    float brick_height = 25.0f;
    float padding = 5.0f;
    int cols = 9;
    int rows = 5;
    float start_x = (800.0f - (cols * (brick_width + padding))) / 2.0f;
    float start_y = 100.0f;

    for (int r = 0; r < rows; ++r) {
      for (int c = 0; c < cols; ++c) {
        auto brick = registry().CreateEntity();
        glm::vec2 pos = {start_x + c * (brick_width + padding),
                         start_y + r * (brick_height + padding)};
        glm::vec4 color = {0.2f + 0.15f * r, 0.8f - 0.15f * r, 0.5f, 1.0f};
        bricks_.push_back({brick, pos, {brick_width, brick_height}});
        registry().AddComponent(brick, BrickComponent{false, color});
      }
    }

    // Particle Emitter
    emitter_entity_ = registry().CreateEntity();
    registry().AddComponent(emitter_entity_, ParticleEmitterComponent{});

    // UI
    score_label_ = registry().CreateEntity();
    ui::UITransform score_trans;
    score_trans.local_pos = {10.0f, 570.0f};
    registry().AddComponent(score_label_, score_trans);
    ui::UIBinding score_binding;
    score_binding.get_text = [this]() {
      return "Bricks Hit: " + std::to_string(bricks_hit_);
    };
    registry().AddComponent(score_label_, score_binding);
    registry().AddComponent(
        score_label_,
        graphics::TextComponent{
            "Bricks Hit: 0", "default", 0.8f, {1.0f, 1.0f, 1.0f, 1.0f}});
  }

  void OnUpdate(float dt) override {
    util::TweenManager::Get().Update(dt);

    if (is_game_over_) {
      if (InputManager::Get().IsKeyPressed(KeyCode::KC_SPACE)) {
        is_game_over_ = false;
        ResetGame();
      }
      return;
    }

    UpdatePaddle(dt);
    UpdateBall(dt);
    UpdateParticles(dt);

    // Decrease screen shake over time
    if (shake_time_ > 0) {
      shake_time_ -= dt;
      if (shake_time_ <= 0) {
        graphics::PostProcessManager::Get().SetShake(0.0f);
      }
    }
  }

  void UpdatePaddle(float dt) {
    auto& paddle = registry().GetComponent<PaddleComponent>(paddle_);
    float move = 0.0f;
    if (InputManager::Get().IsKeyDown(KeyCode::KC_A)) move -= 1.0f;
    if (InputManager::Get().IsKeyDown(KeyCode::KC_D)) move += 1.0f;

    paddle_pos_.x += move * paddle.speed * dt;
    paddle_pos_.x = std::clamp(paddle_pos_.x, paddle.base_size.x / 2.0f,
                               800.0f - paddle.base_size.x / 2.0f);

    // Spring effect
    float target_scale_x = 1.0f + std::abs(move) * 0.2f;
    float target_scale_y = 1.0f - std::abs(move) * 0.2f;

    paddle.current_size.x = paddle.base_size.x * target_scale_x;
    paddle.current_size.y = paddle.base_size.y * target_scale_y;

    // If not moving, spring back
    if (move == 0.0f) {
      paddle.current_size.x +=
          (paddle.base_size.x - paddle.current_size.x) * 10.0f * dt;
      paddle.current_size.y +=
          (paddle.base_size.y - paddle.current_size.y) * 10.0f * dt;
    }
  }

  void UpdateBall(float dt) {
    auto& ball = registry().GetComponent<BallComponent>(ball_);
    ball_pos_ += ball.velocity * dt;

    // Wall collision
    if (ball_pos_.x - ball.radius < 0 || ball_pos_.x + ball.radius > 800.0f) {
      ball.velocity.x *= -1;
      ball_pos_.x = std::clamp(ball_pos_.x, ball.radius, 800.0f - ball.radius);
    }
    if (ball_pos_.y - ball.radius < 0) {
      ball.velocity.y *= -1;
      ball_pos_.y = ball.radius;
    }
    if (ball_pos_.y + ball.radius > 600.0f) {
      // Game Over
      is_game_over_ = true;
      return;
    }

    // Paddle collision
    auto& paddle = registry().GetComponent<PaddleComponent>(paddle_);
    glm::vec2 p_size = paddle.current_size;
    glm::vec2 p_pos = paddle_pos_ - p_size / 2.0f;
    if (CheckCollision(ball_pos_ - glm::vec2(ball.radius),
                       glm::vec2(ball.radius * 2), p_pos, p_size)) {
      ball.velocity.y *= -1;
      ball_pos_.y = paddle_pos_.y - p_size.y / 2.0f - ball.radius;

      // Screen Shake
      graphics::PostProcessManager::Get().SetShake(0.05f);
      shake_time_ = 0.2f;

      // Add a bit of horizontal velocity based on where it hit the paddle
      float hit_pos = (ball_pos_.x - paddle_pos_.x) / (p_size.x / 2.0f);
      ball.velocity.x += hit_pos * 100.0f;
    }

    // Brick collision
    for (auto& b : bricks_) {
      auto& bc = registry().GetComponent<BrickComponent>(b.id);
      if (!bc.is_destroyed) {
        if (CheckCollision(ball_pos_ - glm::vec2(ball.radius),
                           glm::vec2(ball.radius * 2), b.pos, b.size)) {
          bc.is_destroyed = true;
          ball.velocity.y *= -1;
          bricks_hit_++;
          EmitParticles(b.pos + b.size / 2.0f, bc.color);
          break;
        }
      }
    }
  }

  void EmitParticles(glm::vec2 pos, glm::vec4 color) {
    auto& emitter =
        registry().GetComponent<ParticleEmitterComponent>(emitter_entity_);
    for (int i = 0; i < 10; ++i) {
      Particle p;
      p.position = pos;
      float angle = (rand() % 360) * 3.14159f / 180.0f;
      float speed = (rand() % 100) + 50.0f;
      p.velocity = {cos(angle) * speed, sin(angle) * speed};
      p.color = color;
      p.lifetime = 0.5f;
      emitter.particles.push_back(p);
    }
  }

  void UpdateParticles(float dt) {
    auto& emitter =
        registry().GetComponent<ParticleEmitterComponent>(emitter_entity_);
    for (auto it = emitter.particles.begin(); it != emitter.particles.end();) {
      it->position += it->velocity * dt;
      it->lifetime -= dt;
      it->color.a = it->lifetime / 0.5f;
      if (it->lifetime <= 0) {
        it = emitter.particles.erase(it);
      } else {
        ++it;
      }
    }
  }

  void OnRender() override {
    // Draw Background
    graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                       {0.05f, 0.05f, 0.1f, 1.0f});

    if (is_game_over_) {
      graphics::Renderer::Get().DrawText("default", "GAME OVER",
                                         {250.0f, 350.0f}, 0.0f, 2.0f,
                                         {1.0f, 0.0f, 0.0f, 1.0f});
      graphics::Renderer::Get().DrawText("default", "Press SPACE to Restart",
                                         {280.0f, 300.0f}, 0.0f, 1.0f,
                                         {1.0f, 1.0f, 1.0f, 1.0f});
      graphics::Renderer::Get().Flush();
      return;
    }

    // Draw Paddle
    auto& paddle = registry().GetComponent<PaddleComponent>(paddle_);
    graphics::Renderer::Get().DrawQuad(paddle_pos_ - paddle.current_size / 2.0f,
                                       paddle.current_size,
                                       {0.0f, 0.8f, 1.0f, 1.0f});

    // Draw Ball
    auto& ball = registry().GetComponent<BallComponent>(ball_);
    graphics::Renderer::Get().DrawQuad(ball_pos_ - glm::vec2(ball.radius),
                                       glm::vec2(ball.radius * 2),
                                       {1.0f, 1.0f, 0.0f, 1.0f});

    // Draw Bricks
    for (auto& b : bricks_) {
      auto& bc = registry().GetComponent<BrickComponent>(b.id);
      if (!bc.is_destroyed) {
        graphics::Renderer::Get().DrawQuad(b.pos, b.size, bc.color);
      }
    }

    // Draw Particles
    auto& emitter =
        registry().GetComponent<ParticleEmitterComponent>(emitter_entity_);
    for (auto& p : emitter.particles) {
      graphics::Renderer::Get().DrawQuad(p.position, {4.0f, 4.0f}, p.color);
    }

    graphics::Renderer::Get().Flush();
  }

 private:
  struct BrickInfo {
    ecs::EntityID id;
    glm::vec2 pos;
    glm::vec2 size;
  };

  ecs::EntityID paddle_;
  ecs::EntityID ball_;
  ecs::EntityID emitter_entity_;
  ecs::EntityID score_label_;
  std::vector<BrickInfo> bricks_;
  glm::vec2 paddle_pos_;
  glm::vec2 ball_pos_;
  int bricks_hit_ = 0;
  bool is_game_over_ = false;
  float shake_time_ = 0.0f;
};

class MenuScene : public Scene {
 public:
  MenuScene(const std::string& name) : Scene(name) {}

  void OnAttach() override {
    graphics::TextRenderer::Get().Init();
#ifdef COMMON_ASSETS_PATH
    graphics::TextRenderer::Get().LoadFont("default",
                                           COMMON_ASSETS_PATH "arial.ttf", 24);
#else
    graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
#endif
  }

  void OnUpdate(float dt) override {
    if (InputManager::Get().IsKeyDown(KeyCode::KC_SPACE)) {
      SceneManager::Get().SetScene(std::make_unique<GameplayScene>("Gameplay"));
    }
  }

  void OnRender() override {
    // Draw Background
    graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                       {0.05f, 0.05f, 0.1f, 1.0f});

    graphics::Renderer::Get().DrawText("default", "BREAKOUT DEMO",
                                       {200.0f, 400.0f}, 0.0f, 2.0f,
                                       {0.0f, 1.0f, 0.8f, 1.0f});
    graphics::Renderer::Get().DrawText("default", "Press SPACE to Start",
                                       {280.0f, 300.0f}, 0.0f, 1.0f,
                                       {1.0f, 1.0f, 1.0f, 1.0f});
    graphics::Renderer::Get().DrawText("default", "WASD to Move",
                                       {330.0f, 200.0f}, 0.0f, 0.8f,
                                       {0.7f, 0.7f, 0.7f, 1.0f});
    graphics::Renderer::Get().Flush();
  }
};

class BreakoutApp : public Application {
 public:
  void OnInit() override {
    SceneManager::Get().SetScene(std::make_unique<MenuScene>("Menu"));
  }
  void OnShutdown() override {}
  void OnUpdate(double dt) override {
    // Global updates if any
  }
};

int main(void) {
  EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  Engine::Init(config);
  BreakoutApp app;
  app.Run();
  return 0;
}
