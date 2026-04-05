#include <engine/ecs/components/animation.h>
#include <engine/ecs/components/sprite.h>
#include <engine/ecs/systems/ai_system.h>
#include <engine/ecs/components/behavior_tree.h>
#include <engine/ecs/components/state_machine.h>
#include <engine/ecs/components/lifetime.h>
#include <engine/ecs/components/waypoint_path.h>
#include <engine/ecs/components/transform.h>
#include <glm/glm.hpp>
#include <vector>

namespace engine::ecs::systems {

void AISystem::Update(Registry* registry, float dt) {
  if (!registry) {
    return;
  }

  // 1. Update State Machines
  registry->ForEach<engine::ecs::components::StateMachine>(
      [dt](engine::ecs::components::StateMachine& comp) {
        comp.fsm.Update(dt);
      });

  // 2. Update Behavior Trees
  registry->ForEach<engine::ecs::components::BehaviorTree>(
      [dt](engine::ecs::components::BehaviorTree& comp) {
        comp.tree.Tick(dt);
      });

  // 3. Update Animations
  auto anim_view = registry->GetView<engine::ecs::components::Animation, engine::ecs::components::Sprite>();
  for (auto entity : anim_view) {
    auto& anim = registry->GetComponent<engine::ecs::components::Animation>(entity);
    auto& sprite = registry->GetComponent<engine::ecs::components::Sprite>(entity);

    if (!anim.is_playing || anim.current_clip.empty()) {
      continue;
    }

    const auto* clip = anim.GetClip(anim.current_clip);
    if (!clip || clip->frames.empty()) {
      continue;
    }

    anim.timer += dt;
    if (anim.timer >= clip->frames[anim.current_frame].duration) {
      anim.timer = 0.0f;
      anim.current_frame++;

      if (anim.current_frame >= static_cast<int>(clip->frames.size())) {
        if (clip->loop) {
          anim.current_frame = 0;
        } else {
          anim.current_frame = static_cast<int>(clip->frames.size()) - 1;
          anim.is_playing = false;
        }
      }
      sprite.sprite_index = clip->frames[anim.current_frame].sprite_index;
    }
  }

  // 4. Update Lifetime
  std::vector<EntityID> to_destroy;
  auto life_view = registry->GetView<engine::ecs::components::Lifetime>();
  for (auto entity : life_view) {
    auto& life = registry->GetComponent<engine::ecs::components::Lifetime>(entity);
    life.remaining -= dt;
    if (life.remaining <= 0.0f) {
      to_destroy.push_back(entity);
    }
  }

  for (auto entity : to_destroy) {
    registry->DeleteEntity(entity);
  }

  // 5. Update Waypoint Pathing
  auto path_view = registry->GetView<engine::ecs::components::WaypointPath, engine::ecs::components::Transform>();
  for (auto entity : path_view) {
    auto& path = registry->GetComponent<engine::ecs::components::WaypointPath>(entity);
    auto& transform = registry->GetComponent<engine::ecs::components::Transform>(entity);

    if (path.finished || path.points.empty()) {
      continue;
    }

    glm::vec2 target = path.points[path.current_index];
    glm::vec2 direction = target - transform.position;
    float distance = glm::length(direction);

    if (distance <= path.arrival_threshold) {
      path.current_index++;
      if (path.current_index >= static_cast<int>(path.points.size())) {
        if (path.loop) {
          path.current_index = 0;
        } else {
          path.finished = true;
          continue;
        }
      }
      // Re-calculate target for the next waypoint to avoid stuttering this frame
      target = path.points[path.current_index];
      direction = target - transform.position;
      distance = glm::length(direction);
    }

    if (distance > 0.0f) {
      transform.position += (direction / distance) * path.speed * dt;
    }
  }
}

}  // namespace engine::ecs::systems
