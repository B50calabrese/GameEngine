#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <engine/ecs/components/light.h>
#include <engine/ecs/components/occluder.h>
#include <engine/ecs/components/quad.h>
#include <engine/ecs/components/sprite.h>
#include <engine/ecs/components/text.h>
#include <engine/ecs/components/particle_emitter.h>
#include <engine/ecs/components/collider.h>
#include <engine/ecs/components/gravity.h>
#include <engine/ecs/components/velocity.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/post_processor.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/graphics/texture.h>
#include <engine/graphics/utils/particle_system.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/ui/ui_components.h>
#include <engine/ui/ui_systems.h>
#include <engine/util/collision.h>
#include <engine/util/easing.h>
#include <engine/util/tween_manager.h>

#include "../../common/demo_utils.h"
#include "../../common/menu_scene.h"

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

class GameplayScene : public engine::Scene {
 public:
  GameplayScene(const std::string& name) : engine::Scene(name) {}

  void OnAttach() override { ResetGame(); }

  void ResetGame() {
    registry().Clear();
    bricks_hit_ = 0;
    bricks_.clear();

    // Create Paddle
    paddle_ = registry().CreateEntity();
    registry().AddComponent(paddle_, engine::ecs::components::Transform{
                                         {400.0f, 550.0f}, {120.0f, 20.0f}});
    registry().AddComponent(paddle_, engine::ecs::components::Velocity{});
    // Paddle is NOT static so it can move via input, but resolution might move
    // it. However, we set it as trigger to handle collision manually in this
    // demo to preserve bouncing logic.
    registry().AddComponent(
        paddle_, engine::ecs::components::Collider{
                     {120.0f, 20.0f}, {-60.0f, -10.0f}, false, true});
    registry().AddComponent(paddle_, PaddleComponent{});
    registry().AddComponent(
        paddle_, engine::ecs::components::Sprite{
                     "textures/paddle.png", "", 0, {1, 1, 1, 1}, {0.5f, 0.5f}});

    // Create Ball
    ball_ = registry().CreateEntity();
    registry().AddComponent(ball_, engine::ecs::components::Transform{
                                       {400.0f, 500.0f}, {24.0f, 24.0f}});
    registry().AddComponent(
        ball_, engine::ecs::components::Velocity{{300.0f, -300.0f}});
    // Ball is also a trigger to handle its custom "bounce" resolution.
    registry().AddComponent(ball_,
                            engine::ecs::components::Collider{
                                {24.0f, 24.0f}, {-12.0f, -12.0f}, false, true});
    registry().AddComponent(ball_, BallComponent{12.0f});
    registry().AddComponent(
        ball_, engine::ecs::components::Sprite{
                   "textures/ball.png", "", 0, {1, 1, 1, 1}, {0.5f, 0.5f}});

    // Create Bricks
    float brick_width = 75.0f;
    float brick_height = 25.0f;
    float padding = 5.0f;
    int cols = 9;
    int rows = 5;
    float start_x =
        (800.0f - (cols * (brick_width + padding))) / 2.0f + brick_width / 2.0f;
    float start_y = 100.0f + brick_height / 2.0f;

    for (int r = 0; r < rows; ++r) {
      for (int c = 0; c < cols; ++c) {
        auto brick = registry().CreateEntity();
        glm::vec2 pos = {start_x + c * (brick_width + padding),
                         start_y + r * (brick_height + padding)};
        glm::vec4 color = {0.2f + 0.15f * r, 0.8f - 0.15f * r, 0.5f, 1.0f};
        bricks_.push_back({brick, pos, {brick_width, brick_height}});
        registry().AddComponent(brick, engine::ecs::components::Transform{
                                           pos, {brick_width, brick_height}});
        // Bricks are static triggers
        registry().AddComponent(brick,
                                engine::ecs::components::Collider{
                                    {brick_width, brick_height},
                                    {-brick_width / 2.0f, -brick_height / 2.0f},
                                    true,
                                    true});
        registry().AddComponent(brick, BrickComponent{false});
        std::string brick_tex =
            "textures/brick_" + std::to_string(r % 4) + ".png";
        registry().AddComponent(
            brick, engine::ecs::components::Sprite{
                       brick_tex, "", 0, {1, 1, 1, 1}, {0.5f, 0.5f}});
      }
    }

    // Particle Emitter
    emitter_entity_ = registry().CreateEntity();
    registry().AddComponent(emitter_entity_,
                            engine::ecs::components::ParticleEmitter{});

    // UI
    score_label_ = registry().CreateEntity();
    engine::ui::UITransform score_trans;
    score_trans.local_pos = {10.0f, 570.0f};
    registry().AddComponent(score_label_, score_trans);
    engine::ui::UIBinding score_binding;
    score_binding.get_text = [this]() {
      return "Bricks Hit: " + std::to_string(bricks_hit_);
    };
    registry().AddComponent(score_label_, score_binding);
    registry().AddComponent(
        score_label_,
        engine::ecs::components::Text{
            "Bricks Hit: 0", "default", 0.8f, {1.0f, 1.0f, 1.0f, 1.0f}});
  }

  void OnUpdate(float dt) override {
    if (is_game_over_) {
      if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace)) {
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
        engine::graphics::PostProcessManager::Get().SetShake(0.0f);
      }
    }
  }

  void UpdatePaddle(float dt) {
    auto& trans =
        registry().GetComponent<engine::ecs::components::Transform>(paddle_);
    auto& paddle = registry().GetComponent<PaddleComponent>(paddle_);
    float move = 0.0f;
    if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::kA)) {
      move -= 1.0f;
    }
    if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::kD)) {
      move += 1.0f;
    }

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
    auto& trans =
        registry().GetComponent<engine::ecs::components::Transform>(ball_);
    auto& vel =
        registry().GetComponent<engine::ecs::components::Velocity>(ball_);
    auto& ball = registry().GetComponent<BallComponent>(ball_);

    // Wall collision (manual because window bounds aren't entities)
    if (trans.position.x - ball.radius < 0 ||
        trans.position.x + ball.radius > 800.0f) {
      vel.velocity.x *= -1;
      trans.position.x =
          std::clamp(trans.position.x, ball.radius, 800.0f - ball.radius);
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
    auto& p_trans =
        registry().GetComponent<engine::ecs::components::Transform>(paddle_);
    if (engine::util::CheckAabb(
            trans.position - glm::vec2(ball.radius), glm::vec2(ball.radius * 2),
            p_trans.position - p_trans.scale / 2.0f, p_trans.scale)) {
      vel.velocity.y *= -1;
      trans.position.y =
          p_trans.position.y - p_trans.scale.y / 2.0f - ball.radius;

      engine::graphics::PostProcessManager::Get().SetShake(0.05f);
      shake_time_ = 0.2f;

      float hit_pos =
          (trans.position.x - p_trans.position.x) / (p_trans.scale.x / 2.0f);
      vel.velocity.x += hit_pos * 100.0f;
    }

    // Brick collision
    for (auto it = bricks_.begin(); it != bricks_.end();) {
      auto& b = *it;
      if (registry().IsAlive(b.id)) {
        if (engine::util::CheckAabb(trans.position - glm::vec2(ball.radius),
                                    glm::vec2(ball.radius * 2),
                                    b.pos - b.size / 2.0f, b.size)) {
          vel.velocity.y *= -1;
          bricks_hit_++;
          glm::vec4 color = {1, 1, 1, 1};
          EmitParticles(b.pos, color);
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
        registry().GetComponent<engine::ecs::components::ParticleEmitter>(
            emitter_entity_);
    emitter.system.Emit(pos, 10, {0, 0}, {50, 50}, color, 0.5f);
  }

  void OnRender() override {
    engine::graphics::Renderer::Get().DrawQuad(
        {400.0f, 300.0f}, {800.0f, 600.0f}, {0.05f, 0.05f, 0.1f, 1.0f}, 0.0f,
        {0.5f, 0.5f});

    if (is_game_over_) {
      engine::graphics::Renderer::Get().DrawText("default", "GAME OVER",
                                                 {250.0f, 350.0f}, 0.0f, 2.0f,
                                                 {1.0f, 0.0f, 0.0f, 1.0f});
      engine::graphics::Renderer::Get().DrawText(
          "default", "Press SPACE to Restart", {280.0f, 300.0f}, 0.0f, 1.0f,
          {1.0f, 1.0f, 1.0f, 1.0f});
    }

    engine::graphics::Renderer::Get().Flush();
  }

 private:
  struct BrickInfo {
    engine::ecs::EntityID id;
    glm::vec2 pos;
    glm::vec2 size;
  };

  engine::ecs::EntityID paddle_;
  engine::ecs::EntityID ball_;
  engine::ecs::EntityID emitter_entity_;
  engine::ecs::EntityID score_label_;
  std::vector<BrickInfo> bricks_;
  int bricks_hit_ = 0;
  bool is_game_over_ = false;
  float shake_time_ = 0.0f;
};

class BreakoutApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    std::vector<demos::common::BaseMenuScene::MenuItem> items = {
        {"Start Game",
         []() {
           engine::SceneManager::Get().SetScene(
               std::make_unique<GameplayScene>("Gameplay"));
         }},
        {"Quit", []() { engine::Engine::Shutdown(); }}};
    engine::SceneManager::Get().SetScene(
        std::make_unique<demos::common::BaseMenuScene>("BREAKOUT DEMO", items));
  }
};

int main(void) {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  return demos::common::DemoRunner::Run<BreakoutApp>(config);
}
