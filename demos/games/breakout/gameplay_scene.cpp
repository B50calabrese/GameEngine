#include "gameplay_scene.h"

#include <algorithm>

#include <engine/ecs/components/graphics_components.h>
#include <engine/ecs/components/particle_emitter.h>
#include <engine/ecs/components/physics_components.h>
#include <engine/ecs/components/transform.h>
#include <engine/graphics/post_processor.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/ui/ui_components.h>
#include <engine/util/collision.h>

namespace breakout {

GameplayScene::GameplayScene(const std::string& name) : engine::Scene(name) {}

void GameplayScene::OnAttach() { ResetGame(); }

void GameplayScene::ResetGame() {
  GetRegistry().Clear();
  bricks_hit_ = 0;
  bricks_.clear();

  paddle_ = GetRegistry().CreateEntity();
  GetRegistry().AddComponent(paddle_, engine::ecs::components::Transform{
                                          {400.0f, 550.0f}, {120.0f, 20.0f}});
  GetRegistry().AddComponent(paddle_, engine::ecs::components::Velocity{});
  GetRegistry().AddComponent(
      paddle_, engine::ecs::components::Collider{
                   {120.0f, 20.0f}, {-60.0f, -10.0f}, false, true});
  GetRegistry().AddComponent(paddle_, PaddleComponent{});
  GetRegistry().AddComponent(
      paddle_, engine::ecs::components::Quad{{0.0f, 0.8f, 1.0f, 1.0f}});

  ball_ = GetRegistry().CreateEntity();
  GetRegistry().AddComponent(ball_, engine::ecs::components::Transform{
                                        {400.0f, 500.0f}, {20.0f, 20.0f}});
  GetRegistry().AddComponent(
      ball_, engine::ecs::components::Velocity{{300.0f, -300.0f}});
  GetRegistry().AddComponent(
      ball_, engine::ecs::components::Collider{
                 {20.0f, 20.0f}, {-10.0f, -10.0f}, false, true});
  GetRegistry().AddComponent(ball_, BallComponent{});
  GetRegistry().AddComponent(
      ball_, engine::ecs::components::Quad{{1.0f, 1.0f, 0.0f, 1.0f}});

  float brick_width = 75.0f;
  float brick_height = 25.0f;
  float padding = 5.0f;
  int cols = 9;
  int rows = 5;
  float start_x = (800.0f - (cols * (brick_width + padding))) / 2.0f;
  float start_y = 100.0f;

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      auto brick = GetRegistry().CreateEntity();
      glm::vec2 pos = {start_x + c * (brick_width + padding),
                       start_y + r * (brick_height + padding)};
      glm::vec4 color = {0.2f + 0.15f * r, 0.8f - 0.15f * r, 0.5f, 1.0f};
      bricks_.push_back({brick, pos, {brick_width, brick_height}});
      GetRegistry().AddComponent(brick, engine::ecs::components::Transform{
                                            pos, {brick_width, brick_height}});
      GetRegistry().AddComponent(
          brick, engine::ecs::components::Collider{
                     {brick_width, brick_height}, {0, 0}, true, true});
      GetRegistry().AddComponent(brick, BrickComponent{false});
      GetRegistry().AddComponent(brick, engine::ecs::components::Quad{color});
    }
  }

  emitter_entity_ = GetRegistry().CreateEntity();
  GetRegistry().AddComponent(emitter_entity_,
                             engine::ecs::components::ParticleEmitter{});

  score_label_ = GetRegistry().CreateEntity();
  engine::ui::UITransform score_trans;
  score_trans.local_pos = {10.0f, 570.0f};
  GetRegistry().AddComponent(score_label_, score_trans);
  engine::ui::UIBinding score_binding;
  score_binding.get_text = [this]() {
    return "Bricks Hit: " + std::to_string(bricks_hit_);
  };
  GetRegistry().AddComponent(score_label_, score_binding);
  GetRegistry().AddComponent(
      score_label_,
      engine::ecs::components::Text{
          "Bricks Hit: 0", "default", 0.8f, {1.0f, 1.0f, 1.0f, 1.0f}});
}

void GameplayScene::OnUpdate(float dt) {
  if (is_game_over_) {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace)) {
      is_game_over_ = false;
      ResetGame();
    }
    return;
  }

  UpdatePaddle(dt);
  UpdateBall(dt);

  if (shake_time_ > 0) {
    shake_time_ -= dt;
    if (shake_time_ <= 0) {
      engine::graphics::PostProcessManager::Get().SetShake(0.0f);
    }
  }
}

void GameplayScene::UpdatePaddle(float dt) {
  auto& trans =
      GetRegistry().GetComponent<engine::ecs::components::Transform>(paddle_);
  auto& paddle = GetRegistry().GetComponent<PaddleComponent>(paddle_);
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

  float target_scale_x = paddle.base_size.x * (1.0f + std::abs(move) * 0.2f);
  float target_scale_y = paddle.base_size.y * (1.0f - std::abs(move) * 0.2f);

  trans.scale.x += (target_scale_x - trans.scale.x) * 10.0f * dt;
  trans.scale.y += (target_scale_y - trans.scale.y) * 10.0f * dt;
}

void GameplayScene::UpdateBall(float dt) {
  auto& trans =
      GetRegistry().GetComponent<engine::ecs::components::Transform>(ball_);
  auto& vel =
      GetRegistry().GetComponent<engine::ecs::components::Velocity>(ball_);
  auto& ball = GetRegistry().GetComponent<BallComponent>(ball_);

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

  auto& p_trans =
      GetRegistry().GetComponent<engine::ecs::components::Transform>(paddle_);
  if (engine::util::CheckAABB(
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

  for (auto it = bricks_.begin(); it != bricks_.end();) {
    auto& b = *it;
    if (GetRegistry().IsAlive(b.id)) {
      if (engine::util::CheckAABB(trans.position - glm::vec2(ball.radius),
                                  glm::vec2(ball.radius * 2), b.pos, b.size)) {
        vel.velocity.y *= -1;
        bricks_hit_++;
        auto& color = GetRegistry()
                          .GetComponent<engine::ecs::components::Quad>(b.id)
                          .color;
        EmitParticles(b.pos + b.size / 2.0f, color);
        GetRegistry().DeleteEntity(b.id);
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

void GameplayScene::EmitParticles(glm::vec2 pos, glm::vec4 color) {
  auto& emitter =
      GetRegistry().GetComponent<engine::ecs::components::ParticleEmitter>(
          emitter_entity_);
  emitter.system.Emit(pos, 10, {0, 0}, {50, 50}, color, 0.5f);
}

void GameplayScene::OnRender() {
  engine::graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                             {0.05f, 0.05f, 0.1f, 1.0f});

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

}  // namespace breakout
