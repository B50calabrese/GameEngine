#include <engine/core/application.h>
#include <engine/ecs/components/graphics_components.h>
#include <engine/ecs/components/physics_components.h>
#include <engine/ecs/components/transform.h>
#include <engine/graphics/ecs/text_renderer.h>
#include <engine/graphics/renderer.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/collision.h>

#include "level_loader.h"
#include "scenes.h"

namespace platformer {

GameplayScene::GameplayScene(const std::string& name, int level)
    : engine::Scene(name), level_(level) {}

void GameplayScene::OnAttach() { LoadLevel(level_); }

void GameplayScene::LoadLevel(int level) {
  registry().Clear();
  camera_x_ = 0.0f;

  // Create Player
  player_entity_ = registry().CreateEntity();
  registry().AddComponent(player_entity_, engine::ecs::components::Transform{
                                              {50.0f, 100.0f}, {30.0f, 30.0f}});
  registry().AddComponent(player_entity_, engine::ecs::components::Velocity{});
  registry().AddComponent(player_entity_, engine::ecs::components::Gravity{});
  // Player is a physics-resolved object
  registry().AddComponent(
      player_entity_,
      engine::ecs::components::Collider{{30.0f, 30.0f}, {0, 0}, false, false});
  registry().AddComponent(
      player_entity_, engine::ecs::components::Quad{{1.0f, 1.0f, 1.0f, 1.0f}});
  registry().AddComponent(player_entity_, PlayerComponent{});

  std::string level_path = engine::graphics::Renderer::Get().ResolveAssetPath(
      "platformer_level" + std::to_string(level) + ".txt");
  LevelLoader::Load(level_path, registry());
}

void GameplayScene::OnUpdate(float dt) {
  UpdatePlatforms(dt);
  UpdatePlayer(dt);
  UpdateEnemies(dt);
  UpdateCamera();

  auto& player_trans =
      registry().GetComponent<engine::ecs::components::Transform>(
          player_entity_);
  registry().ForEach<GoalComponent, engine::ecs::components::Transform>(
      [this, &player_trans](GoalComponent& gc,
                            engine::ecs::components::Transform& gt) {
        if (engine::util::CheckAABB(player_trans.position, player_trans.scale,
                                    gt.position, gt.scale)) {
          engine::SceneManager::Get().PushScene(
              std::make_unique<LevelCompleteScene>("Complete", level_));
        }
      });
}

void GameplayScene::UpdatePlatforms(float dt) {
  registry()
      .ForEach<PlatformComponent, engine::ecs::components::Transform,
               engine::ecs::components::Velocity>(
          [dt](PlatformComponent& pc, engine::ecs::components::Transform& tc,
               engine::ecs::components::Velocity& vc) {
            if (!pc.active) {
              return;
            }
            if (pc.type == PlatformType::Moving) {
              if (glm::distance(tc.position, pc.start_pos) < 5.0f) {
                vc.velocity =
                    glm::normalize(pc.end_pos - pc.start_pos) * 150.0f;
              }
              if (glm::distance(tc.position, pc.end_pos) < 5.0f) {
                vc.velocity =
                    glm::normalize(pc.start_pos - pc.end_pos) * 150.0f;
              }
            }
            if (pc.type == PlatformType::Temporary && pc.touched) {
              pc.timer += dt;
              if (pc.timer > 2.0f) {
                pc.active = false;
                pc.touched = false;
              }
            }
          });
}

void GameplayScene::UpdatePlayer(float dt) {
  auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
  auto& trans = registry().GetComponent<engine::ecs::components::Transform>(
      player_entity_);
  auto& vel = registry().GetComponent<engine::ecs::components::Velocity>(
      player_entity_);

  if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::kLeft)) {
    vel.velocity.x = -player.move_speed;
  } else if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::kRight)) {
    vel.velocity.x = player.move_speed;
  } else {
    vel.velocity.x = 0;
  }

  // We check if grounded by looking at velocity (simplified)
  // In a real engine we'd check if the collider is resting on something.
  player.is_grounded = (vel.velocity.y == 0);

  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace) &&
      player.is_grounded) {
    vel.velocity.y = player.jump_force;
  }

  // Handle temporary platform touch (as they are triggers/stat-less resolved
  // objects)
  registry().ForEach<PlatformComponent, engine::ecs::components::Transform>(
      [&](PlatformComponent& pc, engine::ecs::components::Transform& tc) {
        if (pc.type == PlatformType::Temporary && pc.active) {
          if (engine::util::CheckAABB(trans.position, trans.scale, tc.position,
                                      tc.scale)) {
            pc.touched = true;
          }
        }
      });

  if (trans.position.x < camera_x_) {
    trans.position.x = camera_x_;
  }
  if (trans.position.y < -100.0f) {
    ResetPlayer();
  }
}

void GameplayScene::UpdateEnemies(float dt) {
  auto& player_trans =
      registry().GetComponent<engine::ecs::components::Transform>(
          player_entity_);
  registry()
      .ForEach<EnemyComponent, engine::ecs::components::Transform,
               engine::ecs::components::Velocity>(
          [this, dt, &player_trans](EnemyComponent& ec,
                                    engine::ecs::components::Transform& tc,
                                    engine::ecs::components::Velocity& vc) {
            if (ec.is_patrolling) {
              if (glm::distance(tc.position, ec.start_pos) < 5.0f) {
                vc.velocity =
                    glm::normalize(ec.end_pos - ec.start_pos) * 100.0f;
              }
              if (glm::distance(tc.position, ec.end_pos) < 5.0f) {
                vc.velocity =
                    glm::normalize(ec.start_pos - ec.end_pos) * 100.0f;
              }
            }
            if (engine::util::CheckAABB(player_trans.position,
                                        player_trans.scale, tc.position,
                                        tc.scale)) {
              ResetPlayer();
            }
          });
}

void GameplayScene::UpdateCamera() {
  auto& player_trans =
      registry().GetComponent<engine::ecs::components::Transform>(
          player_entity_);
  float center_x = camera_x_ + 400.0f;
  if (player_trans.position.x > center_x) {
    camera_x_ = player_trans.position.x - 400.0f;
  }
}

void GameplayScene::ResetPlayer() {
  auto& trans = registry().GetComponent<engine::ecs::components::Transform>(
      player_entity_);
  auto& vel = registry().GetComponent<engine::ecs::components::Velocity>(
      player_entity_);
  trans.position = {50.0f, 100.0f};
  camera_x_ = 0.0f;
  vel.velocity = {0, 0};
  registry().ForEach<PlatformComponent>([](PlatformComponent& pc) {
    if (pc.type == PlatformType::Temporary) {
      pc.active = true;
      pc.touched = false;
      pc.timer = 0.0f;
    }
  });
}

void GameplayScene::OnRender() {
  engine::graphics::Camera& cam = engine::Application::Get().camera();
  cam.set_position({camera_x_, 0.0f, 0.0f});

  engine::graphics::Renderer::Get().DrawQuad(
      {camera_x_, 0.0f}, {800.0f, 600.0f}, {0.1f, 0.1f, 0.2f, 1.0f});

  engine::graphics::Renderer::Get().DrawText(
      "default", "Level: " + std::to_string(level_),
      {camera_x_ + 10.0f, 570.0f}, 0.0f, 0.8f, {1.0f, 1.0f, 1.0f, 1.0f});

  engine::graphics::Renderer::Get().Flush();
}

}  // namespace platformer
