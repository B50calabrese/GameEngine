#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/collision.h>

#include "level_loader.h"
#include "scenes.h"

namespace platformer {

GameplayScene::GameplayScene(const std::string& name, int level)
    : engine::Scene(name), level_(level) {}

void GameplayScene::OnAttach() {
  engine::graphics::TextRenderer::Get().Init();
  engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
  LoadLevel(level_);
}

void GameplayScene::LoadLevel(int level) {
  registry().Clear();
  camera_x_ = 0.0f;

  // Create Player
  player_entity_ = registry().CreateEntity();
  PlayerComponent pc;
  pc.position = {50.0f, 100.0f};
  registry().AddComponent(player_entity_, pc);

  std::string level_path = engine::graphics::Renderer::Get().ResolveAssetPath(
      "platformer_level" + std::to_string(level) + ".txt");
  LevelLoader::Load(level_path, registry());
}

void GameplayScene::OnUpdate(float dt) {
  UpdatePlatforms(dt);
  UpdatePlayer(dt);
  UpdateEnemies(dt);
  UpdateCamera();

  auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
  registry().ForEach<GoalComponent>([this, &player](GoalComponent& gc) {
    if (engine::util::CheckAABB(player.position, player.size, gc.position,
                                gc.size)) {
      engine::SceneManager::Get().PushScene(
          std::make_unique<LevelCompleteScene>("Complete", level_));
    }
  });
}

void GameplayScene::UpdatePlatforms(float dt) {
  registry().ForEach<PlatformComponent>([dt](PlatformComponent& pc) {
    if (!pc.active) return;
    if (pc.type == PlatformType::Moving) {
      pc.position += pc.velocity * dt;
      if (glm::distance(pc.position, pc.start_pos) < 5.0f)
        pc.velocity = glm::normalize(pc.end_pos - pc.start_pos) * 150.0f;
      if (glm::distance(pc.position, pc.end_pos) < 5.0f)
        pc.velocity = glm::normalize(pc.start_pos - pc.end_pos) * 150.0f;
    }
    if (pc.type == PlatformType::Temporary && pc.touched) {
      pc.timer += dt;
      if (pc.timer > 2.0f) pc.active = false;
    }
  });
}

void GameplayScene::UpdatePlayer(float dt) {
  auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
  player.velocity.y -= 1500.0f * dt;
  if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::KC_LEFT))
    player.velocity.x = -player.move_speed;
  else if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::KC_RIGHT))
    player.velocity.x = player.move_speed;
  else
    player.velocity.x = 0;
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::KC_SPACE) &&
      player.is_grounded) {
    player.velocity.y = player.jump_force;
    player.is_grounded = false;
  }
  player.position.y += player.velocity.y * dt;
  player.is_grounded = false;
  registry().ForEach<PlatformComponent>([&player](PlatformComponent& pc) {
    if (!pc.active) return;
    if (engine::util::CheckAABB(player.position, player.size, pc.position,
                                pc.size)) {
      if (player.velocity.y < 0) {
        player.position.y = pc.position.y + pc.size.y;
        player.velocity.y = 0;
        player.is_grounded = true;
        if (pc.type == PlatformType::Temporary) pc.touched = true;
      } else {
        player.position.y = pc.position.y - player.size.y;
        player.velocity.y = 0;
      }
    }
  });
  player.position.x += player.velocity.x * dt;
  registry().ForEach<PlatformComponent>([&player](PlatformComponent& pc) {
    if (!pc.active) return;
    if (engine::util::CheckAABB(player.position, player.size, pc.position,
                                pc.size)) {
      if (player.velocity.x > 0)
        player.position.x = pc.position.x - player.size.x;
      else if (player.velocity.x < 0)
        player.position.x = pc.position.x + pc.size.x;
    }
  });
  if (player.position.x < camera_x_) player.position.x = camera_x_;
  if (player.position.y < -100.0f) ResetPlayer();
}

void GameplayScene::UpdateEnemies(float dt) {
  auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
  registry().ForEach<EnemyComponent>([this, dt, &player](EnemyComponent& ec) {
    if (ec.is_patrolling) {
      ec.position += ec.velocity * dt;
      if (glm::distance(ec.position, ec.start_pos) < 5.0f)
        ec.velocity = glm::normalize(ec.end_pos - ec.start_pos) * 100.0f;
      if (glm::distance(ec.position, ec.end_pos) < 5.0f)
        ec.velocity = glm::normalize(ec.start_pos - ec.end_pos) * 100.0f;
    }
    if (engine::util::CheckAABB(player.position, player.size, ec.position,
                                ec.size)) {
      ResetPlayer();
    }
  });
}

void GameplayScene::UpdateCamera() {
  auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
  float center_x = camera_x_ + 400.0f;
  if (player.position.x > center_x) {
    camera_x_ = player.position.x - 400.0f;
  }
}

void GameplayScene::ResetPlayer() {
  auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
  player.position = {50.0f, 100.0f};
  camera_x_ = 0.0f;
  player.velocity = {0, 0};
  registry().ForEach<PlatformComponent>([](PlatformComponent& pc) {
    if (pc.type == PlatformType::Temporary) {
      pc.active = true;
      pc.touched = false;
      pc.timer = 0.0f;
    }
  });
}

void GameplayScene::OnRender() {
  glm::vec2 offset = {-camera_x_, 0.0f};
  engine::graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                             {0.1f, 0.1f, 0.2f, 1.0f});
  registry().ForEach<PlatformComponent>([offset](PlatformComponent& pc) {
    if (!pc.active) return;
    glm::vec4 color;
    if (pc.type == PlatformType::Stationary)
      color = {0.4f, 0.4f, 0.4f, 1.0f};
    else if (pc.type == PlatformType::Moving)
      color = {0.2f, 0.6f, 0.8f, 1.0f};
    else
      color = {0.8f, 0.4f, 0.2f, 1.0f};
    engine::graphics::Renderer::Get().DrawQuad(pc.position + offset, pc.size,
                                               color);
  });
  registry().ForEach<GoalComponent>([offset](GoalComponent& gc) {
    engine::graphics::Renderer::Get().DrawQuad(gc.position + offset, gc.size,
                                               {1.0f, 1.0f, 0.0f, 1.0f});
  });
  registry().ForEach<EnemyComponent>([offset](EnemyComponent& ec) {
    engine::graphics::Renderer::Get().DrawQuad(ec.position + offset, ec.size,
                                               {1.0f, 0.0f, 0.0f, 1.0f});
  });
  auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
  engine::graphics::Renderer::Get().DrawQuad(
      player.position + offset, player.size, {1.0f, 1.0f, 1.0f, 1.0f});
  engine::graphics::Renderer::Get().DrawText(
      "default", "Level: " + std::to_string(level_), {10.0f, 570.0f}, 0.0f,
      0.8f, {1.0f, 1.0f, 1.0f, 1.0f});
  engine::graphics::Renderer::Get().Flush();
}

}  // namespace platformer
