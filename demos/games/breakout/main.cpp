#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/core/transform.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/graphics_components.h>
#include <engine/graphics/particle_system.h>
#include <engine/graphics/post_processor.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/physics/physics_components.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/ui/ui_components.h>
#include <engine/ui/ui_systems.h>
#include <engine/util/collision.h>
#include <engine/util/easing.h>
#include <engine/util/tween_manager.h>
#include "../../common/menu_scene.h"

using namespace engine;

// --- Components ---

struct PaddleComponent {
  float speed = 500.0f;
  glm::vec2 base_size = {120.0f, 20.0f};
};

struct BallComponent {
  float radius = 10.0f;
};

struct BrickComponent {
  bool is_destroyed = false;
};

// --- Scenes ---

class GameplayScene : public Scene {
 public:
  GameplayScene(const std::string& name) : Scene(name) {}

  void OnAttach() override {
    graphics::TextRenderer::Get().Init();
    graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);

    ResetGame();
  }

  void ResetGame() {
    registry().Clear();
    bricks_hit_ = 0;
    bricks_.clear();

    // Create Paddle
    paddle_ = registry().CreateEntity();
    registry().AddComponent(paddle_, core::TransformComponent{{400.0f, 550.0f}, {120.0f, 20.0f}});
    registry().AddComponent(paddle_, physics::VelocityComponent{});
    // Paddle is NOT static so it can move via input, but resolution might move it.
    // However, we set it as trigger to handle collision manually in this demo to preserve bouncing logic.
    registry().AddComponent(paddle_, physics::ColliderComponent{{120.0f, 20.0f}, {-60.0f, -10.0f}, false, true});
    registry().AddComponent(paddle_, PaddleComponent{});
    registry().AddComponent(paddle_, graphics::QuadComponent{{0.0f, 0.8f, 1.0f, 1.0f}});

    // Create Ball
    ball_ = registry().CreateEntity();
    registry().AddComponent(ball_, core::TransformComponent{{400.0f, 500.0f}, {20.0f, 20.0f}});
    registry().AddComponent(ball_, physics::VelocityComponent{{300.0f, -300.0f}});
    // Ball is also a trigger to handle its custom "bounce" resolution.
    registry().AddComponent(ball_, physics::ColliderComponent{{20.0f, 20.0f}, {-10.0f, -10.0f}, false, true});
    registry().AddComponent(ball_, BallComponent{});
    registry().AddComponent(ball_, graphics::QuadComponent{{1.0f, 1.0f, 0.0f, 1.0f}});

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
        registry().AddComponent(brick, core::TransformComponent{pos, {brick_width, brick_height}});
        // Bricks are static triggers
        registry().AddComponent(brick, physics::ColliderComponent{{brick_width, brick_height}, {0, 0}, true, true});
        registry().AddComponent(brick, BrickComponent{false});
        registry().AddComponent(brick, graphics::QuadComponent{color});
      }
    }

    // Particle Emitter
    emitter_entity_ = registry().CreateEntity();
    registry().AddComponent(emitter_entity_, graphics::ParticleEmitterComponent{});

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

    // Decrease screen shake over time
    if (shake_time_ > 0) {
      shake_time_ -= dt;
      if (shake_time_ <= 0) {
        graphics::PostProcessManager::Get().SetShake(0.0f);
      }
    }
  }

  void UpdatePaddle(float dt) {
    auto& trans = registry().GetComponent<core::TransformComponent>(paddle_);
    auto& paddle = registry().GetComponent<PaddleComponent>(paddle_);
    float move = 0.0f;
    if (InputManager::Get().IsKeyDown(KeyCode::KC_A)) move -= 1.0f;
    if (InputManager::Get().IsKeyDown(KeyCode::KC_D)) move += 1.0f;

    trans.position.x += move * paddle.speed * dt;
    trans.position.x = std::clamp(trans.position.x, paddle.base_size.x / 2.0f,
                                  800.0f - paddle.base_size.x / 2.0f);

    // Spring effect
    float target_scale_x = paddle.base_size.x * (1.0f + std::abs(move) * 0.2f);
    float target_scale_y = paddle.base_size.y * (1.0f - std::abs(move) * 0.2f);

    trans.scale.x += (target_scale_x - trans.scale.x) * 10.0f * dt;
    trans.scale.y += (target_scale_y - trans.scale.y) * 10.0f * dt;
  }

  void UpdateBall(float dt) {
    auto& trans = registry().GetComponent<core::TransformComponent>(ball_);
    auto& vel = registry().GetComponent<physics::VelocityComponent>(ball_);
    auto& ball = registry().GetComponent<BallComponent>(ball_);

    // Wall collision (manual because window bounds aren't entities)
    if (trans.position.x - ball.radius < 0 || trans.position.x + ball.radius > 800.0f) {
      vel.velocity.x *= -1;
      trans.position.x = std::clamp(trans.position.x, ball.radius, 800.0f - ball.radius);
    }
    if (trans.position.y - ball.radius < 0) {
      vel.velocity.y *= -1;
      trans.position.y = ball.radius;
    }
    if (trans.position.y + ball.radius > 600.0f) {
      is_game_over_ = true;
      return;
    }

    // Paddle collision
    auto& p_trans = registry().GetComponent<core::TransformComponent>(paddle_);
    if (util::CheckAABB(trans.position - glm::vec2(ball.radius),
                        glm::vec2(ball.radius * 2),
                        p_trans.position - p_trans.scale / 2.0f,
                        p_trans.scale)) {
      vel.velocity.y *= -1;
      trans.position.y = p_trans.position.y - p_trans.scale.y / 2.0f - ball.radius;

      graphics::PostProcessManager::Get().SetShake(0.05f);
      shake_time_ = 0.2f;

      float hit_pos = (trans.position.x - p_trans.position.x) / (p_trans.scale.x / 2.0f);
      vel.velocity.x += hit_pos * 100.0f;
    }

    // Brick collision
    for (auto it = bricks_.begin(); it != bricks_.end(); ) {
      auto& b = *it;
      if (registry().IsAlive(b.id)) {
        if (util::CheckAABB(trans.position - glm::vec2(ball.radius),
                            glm::vec2(ball.radius * 2), b.pos, b.size)) {
          vel.velocity.y *= -1;
          bricks_hit_++;
          auto& color = registry().GetComponent<graphics::QuadComponent>(b.id).color;
          EmitParticles(b.pos + b.size / 2.0f, color);
          registry().DeleteEntity(b.id);
          it = bricks_.erase(it);
          continue;
        }
      } else {
          it = bricks_.erase(it);
          continue;
      }
      ++it;
    }
  }

  void EmitParticles(glm::vec2 pos, glm::vec4 color) {
    auto& emitter =
        registry().GetComponent<graphics::ParticleEmitterComponent>(emitter_entity_);
    emitter.system.Emit(pos, 10, {0,0}, {50,50}, color, 0.5f);
  }

  void OnRender() override {
    graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                       {0.05f, 0.05f, 0.1f, 1.0f});

    if (is_game_over_) {
      graphics::Renderer::Get().DrawText("default", "GAME OVER",
                                         {250.0f, 350.0f}, 0.0f, 2.0f,
                                         {1.0f, 0.0f, 0.0f, 1.0f});
      graphics::Renderer::Get().DrawText("default", "Press SPACE to Restart",
                                         {280.0f, 300.0f}, 0.0f, 1.0f,
                                         {1.0f, 1.0f, 1.0f, 1.0f});
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
  int bricks_hit_ = 0;
  bool is_game_over_ = false;
  float shake_time_ = 0.0f;
};

class BreakoutApp : public Application {
 public:
  void OnInit() override {
    std::vector<demos::common::BaseMenuScene::MenuItem> items = {
        {"Start Game", []() {
           SceneManager::Get().SetScene(std::make_unique<GameplayScene>("Gameplay"));
         }},
        {"Quit", []() { Engine::Shutdown(); }}
    };
    SceneManager::Get().SetScene(std::make_unique<demos::common::BaseMenuScene>("BREAKOUT DEMO", items));
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
